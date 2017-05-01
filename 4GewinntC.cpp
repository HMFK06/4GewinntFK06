// 4GewinntC.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include <stdlib.h>
#include <iostream>
#include <windows.h>

#define _CRT_SECURE_NO_WARNINGS



char spielFeld[6][7];
char zeichen;
int spieler = 1;
int init(void);
int ergebnis(int sp);
int zeile(void);
int spalte(void);
int ausgabe(int i, int j, int sp);

void Spielzug(void);

void get_computer_move(void)
{
	int i, j;
	spieler *= -1;
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
		for (size_t a = 5; a>0; a--)
		{
			if (spielFeld[a][j] == '0')
			{
				ausgabe(a, j, spieler);
				break;
			}
		}
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


	while ((runde>0) && (ergebnis(spieler) <4)) //Das Spiel läuft so lange bis alle 42 Felder besetzt sind oder ein Spieler gewonnen hat.
	{
		//i = zeile();
		//j = spalte();
		//spieler *= -1;
		//ausgabe(i, j, spieler);
		Spielzug();
		printf("\n");
		runde--;
		printf("Ich denke nach...!\n");
		Sleep(1000);
		get_computer_move();
		printf("\n");
		runde--;
	}
	if (runde>0)
		printf("Sieg!!!\n"); //Hat ein Spieler gewonnen so erschein diese Meldung.

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

	spieler *= -1;
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
	for (j=0 ; j < 6; j++)
	{
		if (r < 4)
			r = 0;
		for (i = 0; i < 6; i++)
		{
			if (spielFeld[i][j] == zeichen)
				r++;
			else if (r < 4)
				r = 0;
		}
	}
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


