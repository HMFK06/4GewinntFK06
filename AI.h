#ifndef AI.h
#define AI


class Intelligenz
{	
private:
	int gewinnChance;
public:
	enum Farbe{Rot = -1, Blau = 1, Leer = 0};
	Farbe Spielerwechsel(Farbe farbe);
	Farbe spieler;
	char spielFeld[6][7];
	int Look_for_win_or_block(int sp);
	bool ausgabe(int iZeile, int jSpalte, int sp);
	
	void computer_makes_move2(void);
	void Spielzug2(void);
	int randomGenerator(int max, int min);
};
#endif