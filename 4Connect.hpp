#include "stdafx.h"
#include <stdlib.h>
#include <iostream>
#include <windows.h>
#include <list>
#include <vector>
#include <map>
#include <stack>
#include <string>
#include <cstdint>
#include <time.h>
#include "TranspositionTable.hpp"
#include <time.h>


#define _CRT_SECURE_NO_WARNINGS

using namespace std;

class Position {
public:

	static const int WIDTH = 7;  // width of the board
	static const int HEIGHT = 6; // height of the board
	static const int MIN_SCORE = -(WIDTH*HEIGHT) / 2 + 3;
	static const int MAX_SCORE = (WIDTH*HEIGHT + 1) / 2 - 3;


	static_assert(WIDTH < 10, "Board's width must be less than 10");
	static_assert(WIDTH*(HEIGHT + 1) <= 64, "Board does not fit in 64bits bitboard");

	/**
	* Indicates whether a column is playable.
	* @param col: 0-based index of column to play
	* @return true if the column is playable, false if the column is already full.
	*/
	bool canPlay(int col) const
	{
		return (mask & top_mask(col)) == 0;
	}

	/**
	* Plays a playable column.
	* This function should not be called on a non-playable column or a column making an alignment.
	*
	* @param col: 0-based index of a playable column.
	*/
	void play(int col)
	{
		current_position ^= mask;
		mask |= mask + bottom_mask(col);
		moves++;
	}



	/*
	* Plays a sequence of successive played columns, mainly used to initilize a board.
	* @param seq: a sequence of digits corresponding to the 1-based index of the column played.
	*
	* @return number of played moves. Processing will stop at first invalid move that can be:
	*           - invalid character (non digit, or digit >= WIDTH)
	*           - playing a colum the is already full
	*           - playing a column that makes an aligment (we only solve non).
	*         Caller can check if the move sequence was valid by comparing the number of
	*         processed moves to the length of the sequence.
	*/
	unsigned int play(std::string seq)
	{
		for (unsigned int i = 0; i < seq.size(); i++) {
			int col = seq[i] - '1';
			if (col < 0 || col >= Position::WIDTH || !canPlay(col) || isWinningMove(col)) return i; // invalid move
			play(col);
		}
		return seq.size();
	}

	/**
	* Indicates whether the current player wins by playing a given column.
	* This function should never be called on a non-playable column.
	* @param col: 0-based index of a playable column.
	* @return true if current player makes an alignment by playing the corresponding column col.
	*/
	bool isWinningMove(int col) const
	{
		uint64_t pos = current_position;
		pos |= (mask + bottom_mask(col)) & column_mask(col);
		return alignment(pos);
	}

	/**
	* @return number of moves played from the beginning of the game.
	*/
	unsigned int nbMoves() const
	{
		return moves;
	}

	/**
	* @return a compact representation of a position on WIDTH*(HEIGHT+1) bits.
	*/
	uint64_t key() const
	{
		return current_position + mask;
	}

	/**
	* Default constructor, build an empty position.
	*/
	Position() : current_position{ 0 }, mask{ 0 }, moves{ 0 } {}



private:
	uint64_t current_position;
	uint64_t mask;
	unsigned int moves; // number of moves played since the beinning of the game.

	/**
	* Test an alignment for current player (identified by one in the bitboard pos)
	* @param a bitboard position of a player's cells.
	* @return true if the player has a 4-alignment.
	*/
	static bool alignment(uint64_t pos) {
		// horizontal 
		uint64_t m = pos & (pos >> (HEIGHT + 1));
		if (m & (m >> (2 * (HEIGHT + 1)))) return true;

		// diagonal 1
		m = pos & (pos >> HEIGHT);
		if (m & (m >> (2 * HEIGHT))) return true;

		// diagonal 2 
		m = pos & (pos >> (HEIGHT + 2));
		if (m & (m >> (2 * (HEIGHT + 2)))) return true;

		// vertical;
		m = pos & (pos >> 1);
		if (m & (m >> 2)) return true;

		return false;
	}

	// return a bitmask containg a single 1 corresponding to the top cel of a given column
	static uint64_t top_mask(int col) {
		return (UINT64_C(1) << (HEIGHT - 1)) << col*(HEIGHT + 1);
	}

	// return a bitmask containg a single 1 corresponding to the bottom cell of a given column
	static uint64_t bottom_mask(int col) {
		return UINT64_C(1) << col*(HEIGHT + 1);
	}

	// return a bitmask 1 on all the cells of a given column
	static uint64_t column_mask(int col) {
		return ((UINT64_C(1) << HEIGHT) - 1) << col*(HEIGHT + 1);
	}

};

class Solver {
private:
	unsigned long long nodeCount; // counter of explored nodes.

	int columnOrder[Position::WIDTH]; // column exploration order

	TranspositionTable transTable;

	int negamax(const Position &P, int alpha, int beta)
	{
		assert(alpha < beta);
		nodeCount++;
		if (P.nbMoves() == Position::WIDTH*Position::HEIGHT)
			return 0;

		for (int x = 0; x < Position::WIDTH; x++)
		if (P.canPlay(x) && P.isWinningMove(x))
			return (Position::WIDTH*Position::HEIGHT + 1 - P.nbMoves()) / 2;

		int max = (Position::WIDTH*Position::HEIGHT - 1 - P.nbMoves()) / 2;	// upper bound of our score as we cannot win immediately
		if (int val = transTable.get(P.key()))
			max = val + Position::MIN_SCORE - 1;

		if (beta > max) {
			beta = max;
			if (alpha >= beta) return beta;
		}
		for (int x = 0; x < Position::WIDTH; x++)
		if (P.canPlay(columnOrder[x])) {
			Position P2(P);
			P2.play(columnOrder[x]);
			int score = -negamax(P2, -beta, -alpha);


			if (score >= beta) return score;
			if (score > alpha) alpha = score;

			transTable.put(P.key(), alpha - Position::MIN_SCORE + 1); // save the upper bound of the position
			return alpha;
		}
	};
public:
	int solve(const Position &P, bool weak = false)
	{
		if (weak)
			return negamax(P, -1, 1);
		else
			return negamax(P, -Position::WIDTH*Position::HEIGHT / 2, Position::WIDTH*Position::HEIGHT / 2);
	}

	unsigned long long getNodeCount()
	{
		return nodeCount;
	}

	void reset()
	{
		nodeCount = 0;
		transTable.reset();
	}
	Solver() : nodeCount{ 0 }, transTable(8388593) { //8388593 prime = 64MB of transposition table
		reset();
		for (int i = 0; i < Position::WIDTH; i++)
			columnOrder[i] = Position::WIDTH / 2 + (1 - 2 * (i % 2))*(i + 1) / 2;
		// initialize the column exploration order, starting with center columns
		// example for WIDTH=7: columnOrder = {3, 4, 2, 5, 1, 6, 0}
	}
};


bool haswon(int64_t board)
{
	int64_t y = board & (board >> 7);
	if (y & (y >> 2 * 7)) // check \ diagonal
		return true;
	y = board & (board >> 8);
	if (y & (y >> 2 * 8)) // check horizontal -
		return true;
	y = board & (board >> 9);
	if (y & (y >> 2 * 9)) // check / diagonal
		return true;
	y = board & (board >> 1);
	if (y & (y >> 2))     // check vertical |
		return true;
	return false;
};

#define BIN64(b63,b62,b61,b60,b59,b58,b57,b56,b55,b54,b53,b52,b51,b50,b49,b48,b47,b46,b45,b44,b43,b42,b41,b40,b39,b38,b37,b36,b35,b34,b33,b32,b31,b30,b29,b28,b27,b26,b25,b24,b23,b22,b21,b20,b19,b18,17,b16,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0)


int main(int argc, char** argv)
{
	Solver solver;
	uint64_t board = 0;
	Position p1;
	p1.play(3);
	p1.play(3);
	p1.play(3);
	p1.play(3);
	p1.isWinningMove(3);
	

	bool weak = false;

	

	if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'w') weak = true;

	std::string line;

	for (int l = 1; std::getline(std::cin, line); l++) {
		Position P;
		if (P.play(line) != line.size())
		{
			std::cerr << "Line " << l << ": Invalid move " << (P.nbMoves() + 1) << " \"" << line << "\"" << std::endl;
		}
		else
		{
			solver.reset();

			int score = solver.solve(P, weak);
			std::cout << line << " " << score << " " << solver.getNodeCount() << " " << endl;
		}
		std::cout << std::endl;
	}

};