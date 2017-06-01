#include <stdio.h>
#include "AI.h"
#include "stdafx.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>

int gewinnChance2 = 0;
int randomGenerator2(int max, int min)
{
	srand(time(NULL));
	int random = rand() % max + min;
	return random;
}


int Spielerwechsel2(int sp)
{
	if (sp==1)
	{
		return -1;
	}
	else if (sp == -1)
	{
		return 1;
	}
}

int spieler2;

char spielFeld2[6][7];

int Look_for_win_or_block2(int sp)
{
	int zeichen;
	gewinnChance2 = 0;
	int bestPlace;
	int i, j, r = 0;
	if (sp < 0)
		zeichen = 'A';
	else
		zeichen = 'B';

	for (j = 5; j >0; j--)
	{
		for (i = 6; i >= 0; i--)
		{
			//horizontal // gewinnChance2 sehr hoch!!!!
			if (spielFeld2[j][i] == zeichen && spielFeld2[j][i - 1] == zeichen && spielFeld2[j][i - 2] == zeichen)
			{
				//(spielFeld2[j][i - 3] == '0' || spielFeld2[j][i + 1] == '0')
				if (spielFeld2[j][i - 3] == '0' && i > 2)
				{
					gewinnChance2 = 1;
					return i - 3;

				}
				if (spielFeld2[j][i + 1] == '0' && i < 6)
				{
					gewinnChance2 = 1;
					return i + 1;

				}
			}
			//vertikal
			if (spielFeld2[j][i] == zeichen && spielFeld2[j - 1][i] == zeichen && spielFeld2[j - 2][i] == zeichen && spielFeld2[j - 3][i] == '0')
			{
				gewinnChance2 = 1;
				return i;
			}
			//Example _XX_
			if (spielFeld2[j][i - 1] == '0' && spielFeld2[j][i] == zeichen && spielFeld2[j][i + 1] == zeichen && spielFeld2[j][i + 2] == '0' && i>0)
			{
				return i - 1;
			}
			//Example X_XX
			if (spielFeld2[j][i] == zeichen && spielFeld2[j][i + 2] == zeichen && spielFeld2[j][i + 3] == zeichen && i<4)
			{
				gewinnChance2 = 1;
				return j;

			}
			//Example _XXX
			//Example _eee
			if (spielFeld2[j][i - 1] == '0' && spielFeld2[j][i] == zeichen && spielFeld2[j][i + 1] == zeichen)
			{
				if (spielFeld2[j][i + 2] == zeichen && i<5)
					return j;
			}

			//Diagonal

			if (spielFeld2[j][i] == zeichen && spielFeld2[j - 1][i + 1] == zeichen && spielFeld2[j - 2][i + 2] == zeichen)
			{
				gewinnChance2 = 1;
				return j - 3;

			}
		}
	}
	return NULL;
}

bool ausgabe2(int iZeile, int jSpalte, int sp)
{
	int a;
	int b;
	if (sp<0)
		spielFeld2[iZeile][jSpalte] = 'A';
	if (sp>0)
		spielFeld2[iZeile][jSpalte] = 'B';
	printf("\n");
	for (a = 0; a<7; a++)
	{
		//printf("\n");
		//for (b = 0; b<7; b++)
		//	printf(" %c", spielFeld2[a][b]);
	}
	return true;
}

void computer_makes_move2()
{
	int i, j;
	int farbe = 1;
	bool ausgegeben = true;
	int random_spalte;
	int spalte_move;
	for (size_t a = 5; a > 0; a--)
	{
		if (ausgegeben)
		{
			spalte_move = Look_for_win_or_block2(-1);
			int SpielerChance = gewinnChance2;
			int test_spalte_move = Look_for_win_or_block2(spieler2);
			int ComputerChance = gewinnChance2;
			if (SpielerChance < ComputerChance)
				spalte_move = -1;
			if (spalte_move < NULL)
			{
				spalte_move = Look_for_win_or_block2(spieler2);
				if (spalte_move < NULL)
				{
					spalte_move = randomGenerator2(6, 0);
				}

			}
		}
		if (spielFeld2[a][spalte_move] == '0')
		{
			ausgegeben = ausgabe2(a, spalte_move, spieler2);
			break;
		}
		else
			ausgegeben = false;
	}
}

void Spielzug2(void)
{
	//int x=0;
	//do
	//{
	//	//printf("Spalte: ");
	//	//int eingabe = scanf_s("%d", &x);
	//} while (x>6);

	//for (size_t i = 5; i>0; i--)
	//{
	//	if (spielFeld2[i][x] == '0')
	//	{
	//		ausgabe2(i, x, spieler2);
	//		break;
	//	}
	//}
}






