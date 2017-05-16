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

#define _CRT_SECURE_NO_WARNINGS

using namespace std;

//asdklfjasdklfjasdöklajfsdklfjsd öklsdjföklsdfjs öklfsdjöflas
enum Farbe{ Rot = 1, Blau = -1, Leer = 0 };

char spielFeld[6][7];
char zeichen;
Farbe spieler = Rot;
int init(void);
int ergebnis(int sp);
int ausgabe(int i, int j, int sp);
int erg;
int GetBestMove(Farbe sp, char spielFeld[6][7]);
vector<pair<char,char>> openSpaces;
stack<int, vector<int>> op;
int runde;



void Spielzug(void);


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


void get_computer_move(void)
{
	int i, j;
	int farbe = Rot;
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 6; j++)
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
		for (size_t a = 5; a > 0; a--)
		{
			int random_spalte = rand() % 5 + 1;
			if (spielFeld[a][random_spalte] == '0')
			{
				ausgabe(a, random_spalte, spieler);
			}
		}
		}
		
	}			


void Simulate(int wieOft)
{
	int runde = 42;
	int a_gewonnen = 0;
	int b_gewonnen = 0;
	int unentschieden = 0;
	spieler = Spielerwechsel(Rot);
	for (size_t i = 0; i < wieOft; i++)
	{
		do
		{
			spieler = Spielerwechsel(spieler);
			get_computer_move();
			runde--;
			erg = ergebnis(spieler);
			if (erg > 4)
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

		printf("\nA gewonnen: %d \n B gewonnen: %d\n Untenschieden: %d", a_gewonnen, b_gewonnen, unentschieden);

	}
	

}

int _tmain(int argc, _TCHAR* argv[])
{
	openSpaces.push_back(make_pair('2', '3'));
	pair<char,char> a = openSpaces[0];
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


	do
	{
		spieler = Spielerwechsel(spieler);
		Spielzug();
		printf("\n");
		runde--;
		erg = ergebnis(spieler);
		if (erg > 4)
		{
			break;
		}
		spieler = Spielerwechsel(spieler);
		printf("Ich denke nach...!\n");
		//Sleep(1000);
		get_computer_move();
		erg = ergebnis(spieler);
		printf("\n");
		runde--;
	} while (runde > 0 && erg<4);
	
	if (runde>0)
	{

		if (spieler > 0)
			printf("Sieg!!! Spieler Rot hat gewonnen\n"); //Hat ein Spieler gewonnen so erschein diese Meldung.
		else
			printf("Sieg!!! Spieler Blau hat gewonnen\n");
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
	return 0;
}


void Spielzug(void)
{
	int x;
	printf("Spalte: ");
	scanf_s("%d", &x);


	for (size_t i = 5; i>0; i--)
	{
		if (spielFeld[i][x] == '0')
		{
			ausgabe(i, x, spieler);
			break;
		}
	}
}

int ergebnis(int sp)
{
	char zeichnen;
	int i, j, r = 0;
	if (sp < 0)
		zeichen = 'A';
	else
		zeichen = 'B';
	//Zeilen
	//zeichen = 'A';
	for (j=0 ; j < 6; j++)
	{
		if (r < 4)
			r = 0;
		for (i = 0; i < 6; i++)
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
		for (i = 0; i < 7; i++)
		{

			if (spielFeld[j][i] == zeichen)
				r++;
			else if (r < 4)
				r = 0;
		}
	}

	return r;
}

int ausgabe(int i, int j, int sp)
{
	int a;
	int b;
	if (sp<0)
		spielFeld[i][j] = 'A';
	if (sp>0)
		spielFeld[i][j] = 'B';
	printf("\n");
	for (a = 0; a<7; a++)
	{
		printf("\n");
		for (b = 0; b<7; b++)
			printf(" %c", spielFeld[a][b]);
	}
	return 0;
}

////pair<char,char> GetBestMove(Farbe sp, char spielFeld[6][7])
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