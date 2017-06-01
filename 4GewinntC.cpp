// 4GewinntC.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include <stdlib.h>
#include <iostream>
#include <windows.h>
#include <list>
#include <vector>
#include <map>
#include <stack>
#include <time.h>
#include <algorithm>
#include "AI.h"


#define _CRT_SECURE_NO_WARNINGS

using namespace std;

enum Farbe{ Rot = 1, Blau = -1, Leer = 0 };

char spielFeld[6][7];
char zeichen;
Farbe spieler = Rot;
int init(void);
int ergebnis(int sp);
bool ausgabe(int i, int j, int sp);
int erg;
pair<char, char> GetBestMove(Farbe sp, char spielFeld[6][7]);
vector<pair<char, char>> openSpaces;
stack<int, vector<int>> op;
int runde;
int minimax(char spielFeld[6][7], Farbe spieler);
void computerMove(char spielFeld[6][7]);
int Look_for_win_or_block(Farbe spieler);
void computer_makes_move(void);
int play_randomly(char spielFeld[6][7]);
int TrytoWin(Farbe sp);
void simulate_plays(char spielFeld[6][7], int number);
void Spielzug(void);
int gewinnChance = 0;
int openSpace[7];
Intelligenz a;

Farbe Spielerwechsel(Farbe farbe)
{

	if (farbe == Rot)
	{
		return Blau;
	}
	else if (farbe == Blau)
	{
		return Rot;
	}

}

int randomGenerator(int max, int min)
{
	srand(time(NULL));
	int random = rand() % max + min;
	return random;
}

void get_computer_move(void)
{
	int i, j;
	int farbe = Rot;
	for (i = 0; i < 6; i++)
	{
		for (j = 0; j < 7; j++)
		{
			if (spielFeld[i][j] == '0')
				break;
		}
	}
	if (i*j == 42)  {
		printf("draw\n");
		exit(0);
	}
	else
	{
		bool ausgegeben = true;
		int random_spalte;
		for (size_t a = 5; a > 0; a--)
		{
			if (ausgegeben)
				//random_spalte = TrytoWin(spieler);
				random_spalte = Look_for_win_or_block(spieler);
			if (spielFeld[a][random_spalte] == '0')
			{
				ausgegeben = ausgabe(a, random_spalte, spieler);
				break;
			}
			else
				ausgegeben = false;
			
		}
	}
}

void computer_makes_move(void)
{
	
	int i, j;
	int farbe = Rot;
	//for (i = 0; i < 6; i++)
	//{
	//	for (j = 0; j < 7; j++)
	//	{
	//		if (spielFeld[i][j] == '0')
	//			break;
	//	}
	//}
	//if (i*j == 42)  {
	//	printf("draw\n");
	//	exit(0);
	//}
	//else
	//{
		bool ausgegeben = true;
		int random_spalte;
		int spalte_move;

#pragma region test
		//for (size_t a = 5; a >= 0; a--)
		//{
		//	if (ausgegeben)
		//	{

		//		//random_spalte = TrytoWin(spieler);
		//		spalte_move = Look_for_win_or_block(Blau);
		//		int SpielerChance = gewinnChance;
		//		int test_spalte_move = Look_for_win_or_block(spieler);
		//		int ComputerChance = gewinnChance;
		//		if (SpielerChance < ComputerChance)
		//			spalte_move = -1;
		//		if (spalte_move < NULL)
		//		{
		//			spalte_move = Look_for_win_or_block(spieler);
		//			if (spalte_move < NULL)
		//			{
		//				spalte_move = randomGenerator(6, 0);
		//			}

		//		}
		//	}
		//	if (spielFeld[a][spalte_move] == '0')
		//	{
		//		ausgegeben = ausgabe(a, spalte_move, spieler);
		//		break;
		//	}
		//	else
		//		ausgegeben = false;
		//}
#pragma endregion

		do
		{
			if (ausgegeben)
			{
				//random_spalte = TrytoWin(spieler);
				spalte_move = Look_for_win_or_block(Blau);
				int SpielerChance = gewinnChance;
				int test_spalte_move = Look_for_win_or_block(spieler);
				int ComputerChance = gewinnChance;
				if (SpielerChance < ComputerChance)
					spalte_move = -1;
				if (spalte_move < NULL)
				{
					spalte_move = Look_for_win_or_block(spieler);
					if (spalte_move < NULL)
					{
						spalte_move = randomGenerator(6, 0);
					}
				}
			}
			if (openSpace[spalte_move] >= 0)
			{
				ausgegeben = ausgabe(openSpace[spalte_move], spalte_move, spieler);
				openSpace[spalte_move]--;
				break;
			}
		} while (openSpace[spalte_move]>=0);
	//}
}


void Simulate(int wieOft)
{
	int runde = 42;
	int a_gewonnen = 0;
	int b_gewonnen = 0;
	int unentschieden = 0;
	spieler = Spielerwechsel(Blau);
	for (size_t i = 0; i < wieOft; i++)
	{
		do
		{
			spieler = Spielerwechsel(spieler);
			get_computer_move();
			runde--;
			erg = ergebnis(spieler);
			if (erg >= 4)
			{
				break;
			}
			spieler = Spielerwechsel(spieler);
			get_computer_move();
			erg = ergebnis(spieler);
			runde--;
		} while (runde > 0 && erg<4);
		if (runde>0)
		{
			if (spieler > 0)
				b_gewonnen++;
			else
				a_gewonnen++;
		}
		else
			unentschieden++;

		printf("\nA gewonnen: %d \nB gewonnen: %d\nUntenschieden: %d\n", a_gewonnen, b_gewonnen, unentschieden);

	}


}

int _tmain(int argc, _TCHAR* argv[])
{


	int runde = 42; //42 ergibt sich aus der maximal Anzahl Felder die das Spielfeld hat. 7*6=42.
	int i;
	int j;
	//Diese Variabel gibt den aktuellen Spieler an. 1 steht für Spieler B (=Spieler 2) und -1 steht für Spieler A (=Spieler 1).
	init();


	printf("4 Gewinnt\n"); //Titel
	printf("---------\n\n");
	printf("Info:\n Spieler1 hat den Spielstein A und Spieler 2 hat den Spielstein B.\n Spieler 1 beginnt.\n\n");

	printf("Spielfeldaufbau:\n\n"); //Aufbau des Spielfeldes und wie die Zeilen und Spalten beziefert sind.

	printf("      S p a l t e n\n");
	printf("      0 1 2 3 4 5 6 \n");
	printf("      | | | | | | |\n");
	printf(" Z 0- 0 0 0 0 0 0 0\n");
	printf(" e 1- 0 0 0 0 0 0 0\n");
	printf(" i 2- 0 0 0 0 0 0 0\n");
	printf(" l 3- 0 0 0 0 0 0 0\n");
	printf(" e 4- 0 0 0 0 0 0 0\n");
	printf(" n 5- 0 0 0 0 0 0 0\n\n");
	//Simulate(1);

	do
	{
		spieler = Spielerwechsel(spieler);
		Spielzug();
		printf("\n");
		runde--;
		erg = ergebnis(spieler);
		if (erg >= 4)
		{
			break;
		}
		
		spieler = Spielerwechsel(spieler);
		printf("Ich denke nach...!\n");
		//Sleep(1000);
		//get_computer_move();
		
		computer_makes_move();
		erg = ergebnis(spieler);
		printf("\n");
		runde--;
	} while (runde > 0 && erg<4);
	
	if (runde>0)
	{

		if (spieler > 0)
			printf("Sieg!!! Spieler Rot hat gewonnen\n"); //Hat ein Spieler gewonnen so erschein diese Meldung.
		else if (spieler<0)
			printf("Sieg!!! Spieler Blau hat gewonnen\n");
	}
	else{
		printf("Unentschieden!!!");
	}
	system("pause"); //damit sich das Programm nicht selber beendet wen das Spiel fertig ist.
	return 0;
}


int init(void)
{
	int x;
	int y;
	for (x = 0; x < 6; x++)
	{
		for (y = 0; y < 7; y++)
		{
			spielFeld[x][y] = '0';
		}
	}

	for (size_t i = 0; i < 7; i++)
	{
		openSpace[i] = 5;
	}

	return 0;


}



int Look_for_win_or_block(Farbe sp)
{
	gewinnChance = -1;
	int bestPlace;
	int i, j, r = 0;
	if (sp < 0)
		zeichen = 'X';
	else
		zeichen = 'R';

	for (j = 5; j >0; j--)
	{
		for (i = 6; i >= 0; i--)
		{
			//horizontal // Gewinnchance sehr hoch!!!!
			if (spielFeld[j][i] == zeichen && spielFeld[j][i - 1] == zeichen && spielFeld[j][i - 2] == zeichen)
			{
				//(spielFeld[j][i - 3] == '0' || spielFeld[j][i + 1] == '0')
				if (spielFeld[j][i - 3] == '0' && i > 2)
				{
					gewinnChance = 1;
					return i - 3;
					
				}
				if (spielFeld[j][i + 1] == '0' && i < 6)
				{
					gewinnChance = 1;
					return i + 1;
					
				}
			}
			//vertikal
			if (spielFeld[j][i] == zeichen && spielFeld[j - 1][i] == zeichen && spielFeld[j - 2][i] == zeichen && spielFeld[j - 3][i] == '0')
			{
				gewinnChance = 1;
				return i;
			}
			//Example _XX_
			if (spielFeld[j][i - 1] == '0' && spielFeld[j][i]==zeichen && spielFeld[j][i+1]==zeichen && spielFeld[j][i + 2] == '0' && i>0)
			{
				return i - 1;
			}
			//Example X_XX
			if (spielFeld[j][i] == zeichen && spielFeld[j][i + 2] == zeichen && spielFeld[j][i + 3] == zeichen && spielFeld[j][i+1] && i<4)
			{
					gewinnChance = 1;
					return i+1;

			}
			//Example XX_X
			if (spielFeld[j][i] == zeichen && spielFeld[j][i + 1] == zeichen && spielFeld[j][i + 3] == zeichen && spielFeld[j][i+2]=='0' && i<4)
			{
				gewinnChance = 1;
				return i+2;

			}
			//Example _XXX
			//Example _eee
			if (spielFeld[j][i-1] == '0' && spielFeld[j][i] == zeichen && spielFeld[j][i + 1] == zeichen)
			{
				if (spielFeld[j][i + 2] == zeichen && i<5)
				return j;
			}


			//Diagonal

			if (spielFeld[j][i] == zeichen && spielFeld[j - 1][i + 1] == zeichen && spielFeld[j - 2][i + 2] == zeichen && spielFeld[j-3][i+3]=='0')
			{
				gewinnChance = 1;
				return i+3;
			}

			if (spielFeld[j][i] == zeichen && spielFeld[j - 1][i - 1] == zeichen && spielFeld[j - 2][i - 2] == zeichen && spielFeld[j - 3][i - 3] == '0' && i>4)
			{
				gewinnChance = 1;
				return i- 3;
			}
		}
	}
	return -1;	
}

void Spielzug(void)
{
	int x;
	do
	{
		printf("Spalte: ");
		int eingabe = scanf_s("%d", &x);
		if (openSpace[x] < 0)
			x = 7;
	} while (x>6);
	ausgabe(openSpace[x], x, spieler);
	openSpace[x]--;
	//for (size_t i = 5; i>0; i--)
	//{
	//	if (spielFeld[i][x] == '0')
	//	{
	//		ausgabe(i, x, spieler);
	//		openSpace[x]--;
	//		break;
	//	}
	//}
	
}

int ergebnis(int sp)
{
	char zeichnen;
	int i, j, r = 0;
	if (sp < 0)
		zeichen = 'X';
	else
		zeichen = 'R';
	//Zeilen
	//zeichen = 'A';
	for (j = 0; j < 6; j++)
	{
		if (r < 4)
			r = 0;
		for (i = 0; i < 7; i++)
		{
			if (spielFeld[j][i] == zeichen)
			{
				r++;
				if (r >= 4)
				{
					break;
				}
			}

			else if (r < 4)
				r = 0;
		}
	}
	//Spalten
	for (j = 0; j < 7; j++)
	{
		if (r < 4)
			r = 0;
		for (i = 0; i < 6; i++)
		{

			if (spielFeld[i][j] == zeichen)
			{

				r++;
				if (r >= 4)
				{
					break;
				}
			}

			else if (r < 4)
				r = 0;
		}
	}

	//Diagonal
	for (j = 5; j > 0; j--)
	{
		if (r < 4)
			r = 0;
		for (i = 6; i > 0; i--)
		{

			if (spielFeld[j][i] == zeichen && spielFeld[j-1][i+1] == zeichen && spielFeld[j-2][i+2] == zeichen & spielFeld[j-3][i+3] == zeichen)
			{
				r = 4;
				break;
			}

			else if (r < 4)
				r = 0;
		}
	}

	return r;
}

bool ausgabe(int i, int j, int sp)
{
	int a;
	int b;
	if (sp<0)
		spielFeld[i][j] = 'X';
	if (sp>0)
		spielFeld[i][j] = 'R';
	printf("\n");
	for (a = 0; a<7; a++)
	{
		printf("\n");
		for (b = 0; b<7; b++)
			printf(" %c", spielFeld[a][b]);
	}
	return true;
}

//pair<char,char> GetBestMove(Farbe sp, char spielFeld[6][7])
//{
//	char bestSpace[1][1];
//
//	char newBoard[6][7];
//
//	char zeichen;
//
//	if (sp < 0)
//		zeichen = 'A';
//	else
//		zeichen = 'B';
//
//	for (size_t i = 0; i < sizeof(openSpaces); i++)
//	{
//		memcpy(newBoard, spielFeld, sizeof(spielFeld)); //Spielfeld kopieren
//
//		pair<char, char> newSpace = openSpaces[i];
//		newBoard[newSpace.first][newSpace.second] = sp;
//		runde--;
//		if (ergebnis(spieler)<4)
//		{
//			pair<char, char> tempMove = GetBestMove(Spielerwechsel(spieler), newBoard);
//
//
//	}
//
//
//	
//
//	}
//
//	return NULL;
//}

int minimax(char spielFeld[6][7], Farbe spieler)
{
	int winner = ergebnis(spieler);
	if (winner != 0) return winner*spieler;
	if (spieler < 0)
		zeichen = 'A';
	else
		zeichen = 'B';
	int move = -1;
	int score = -2;
	int i;
	int j;
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 6; j++)
		{
			if (spielFeld[i][j] == '0')
			{
				spielFeld[i][j] = zeichen;
				int thisScore = -minimax(spielFeld, Spielerwechsel(spieler));
				if (thisScore>score)
				{
					score = thisScore;
					move = i;
				}
				spielFeld[i][j] = 0;
			}
		}
		if (move == -1) return 0;
		return score;
	}
}
void computerMove(char spielFeld[6][7])
{
	int move = -1;
	int score = -2;
	int i;
	int j;
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 6; j++)
		{
			if (spielFeld[i][j] == '0')
			{
				spielFeld[i][j] = 1;
				int tempScore = -minimax(spielFeld, Rot);
				if (tempScore>score)
				{
					score = tempScore;
					move = i;
				}
				spielFeld[i][j] = 0;
			}
			ausgabe(i, j, 1);
		}

	}

}

int play_randomly(char spielFeld[6][7])
{
	int value = ergebnis(spieler);
	int moves = 42;
	int a_gewonnen = 0;
	int b_gewonnen = 0;
	int unentschieden = 0;
	int count[1][1][1];
	count[0][0][0] = 0;
	while (value == 0 && moves>0)
	{

		if (moves == 0) return 0;
		get_computer_move();
		moves--;
		value = ergebnis(spieler);
		if (value >= 4)
		{
			string gewinner = (spieler = Rot) ? "Rot" : "Blau";
			printf("Spieler %s hat gewonnen !\n", gewinner);
			if (spieler == Rot)
				b_gewonnen++;

			else if (spieler == Blau)
				a_gewonnen++;
			else
				unentschieden++;
			break;
		}
		spieler = Spielerwechsel(spieler);
	}
	count[0][0][0] = a_gewonnen, b_gewonnen, unentschieden;
	//cout << count << endl;
	return value;
}

void simulate_plays(char spielFeld[6][7], int number)
{
	while (number > 0)
	{
		char newBoard[6][7];
		//copy_n(spielFeld, 42, newBoard);
		newBoard[6][7] = spielFeld[6][7];
		play_randomly(newBoard);
		number--;
	}



}