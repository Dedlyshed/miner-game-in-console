#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#define uncheckedopen  -1
#define empty			0
#define opened			1
#define mine			2
#define markedempty		3
#define markedmine		4

//Made by Dedlyshed 16.01.2021

void fieldClear(int* field, int Xsize, int Ysize) { //очищает поле
	for (int y = 0; y < Ysize; y++) {
		for (int x = 0; x < Xsize; x++) {
			*(field + x + Xsize * y) = empty;
		}
	}
}


void fieldAddMines(int* field, int Xsize, int Ysize, int amount) { //создает amount мин в радномных местах на поле 
	srand(time(NULL)); int x, y;
	for (int i = 0; i < amount; i++) {
		while (1) {
			x = rand() % (Xsize);
			y = rand() % (Ysize);
			if (*(field + x + Xsize * y) != mine) break;
		}
		*(field + x + Xsize * y) = mine;
	}
}

int checkNeighbors(int* field, int Xsize, int Ysize, int y, int x) { //возвращает колличество мин рядом с клеткой
    int neighbors = 0;
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
			if (x + j >= Xsize || x + j < 0 || y + i >= Ysize || y + i < 0) continue;
			if (*(field + x+j + Xsize * (y+i)) == mine || *(field + x + j + Xsize * (y + i)) == markedmine) {
                if (y != 0 || x != 0) {
                    neighbors++;
                }
            }
        }
    }
    return neighbors;
}

void fieldPrint(int* field, int Xsize, int Ysize, int showMines) { //выводит поле в консоль
	for (int y = 0; y < Ysize; y++) {
		for (int x = 0; x < Xsize; x++) {
			if		(*(field + x + Xsize * y) == empty) printf(": ");
			else if (*(field + x + Xsize * y) == opened || *(field + x + Xsize * y) == uncheckedopen) printf("%d ", checkNeighbors(field, Xsize, Ysize, y, x));
			else if (*(field + x + Xsize * y) == mine && showMines == 1) printf("* ");
			else if (*(field + x + Xsize * y) == mine)printf(": ");
			else if (*(field + x + Xsize * y) == markedmine)printf("! ");
		}
		printf("\n");
	}
}

void checkNulls(int* field, int Xsize, int Ysize, int y, int x) { //проверяет колличество нулей в соседних клетках, моментально их открывая
	COORD position;										// Объявление необходимой структуры для перемещение каретки
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  // Получение дескриптора устройства стандартного вывода
	*(field + x + Xsize * y) = opened;
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			if (x + j >= Xsize || x + j < 0 || y + i >= Ysize || y + i < 0) continue;
			int neighbours = checkNeighbors(field, Xsize, Ysize, y, x);
			if (neighbours == 0 && *(field + x + j + Xsize * (y + i)) != opened) {
				*(field + x + j + Xsize * (y + i)) = opened;
				position.X = 2 * (x+j);                               
				position.Y = y+i;
				printf("\b ");
				SetConsoleCursorPosition(hConsole, position);
				neighbours = checkNeighbors(field, Xsize, Ysize, y+i, x+j);
				printf("%d ", neighbours);
				checkNulls(field, Xsize, Ysize, y + i, x + j);
			}
		}
	}
}

int main() {
	int *fieldArr, minesAmount, menu;
	int  showmines = 0, markedmines = 0, markers, gameover = 0;	
	int timestart, timeend, timeplayedsec, timeplayedmin = 0;	//Для счета времени в игре
	int x = 0, y = 0, xy, Xsize, Ysize;		//Xsize, Ysize - размер поля; x, y - координаты "курсора"; xy - считывание управления
	printf("Welcome to miner game!\n");
	printf("Made by Mahurin Alexey\n");
	printf("Choose difficulty:\n1 - easy   (9x9, 10 mines)\n2 - normal (16x16, 40 mines)\n3 - hard   (16x30, 100 mines)\n");
	scanf_s("%d", &menu);
	switch (menu) {
	default: printf("Incorect choose, seting up to easy mode.");
	case 1: Xsize = 9; Ysize = 9; minesAmount = 10; break;
	case 999: showmines = 1;
	case 2: Xsize = 16; Ysize = 16; minesAmount = 40; break;
	case 3: Xsize = 30; Ysize = 16; minesAmount = 100; break;
	}
	markers = minesAmount;
	system("cls");
	fieldArr = (int*)calloc(Xsize * Ysize, sizeof(int));
	//*(fieldArr + x + Xsize * y); - обращение к [x;y] элементу
	fieldClear(fieldArr, Xsize, Ysize);
	fieldAddMines(fieldArr, Xsize, Ysize, minesAmount);
	fieldPrint(fieldArr, Xsize, Ysize, showmines);
	COORD position;										// Объявление необходимой структуры для перемещение каретки
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  // Получение дескриптора устройства стандартного вывода
	timestart = time(NULL);
	while (gameover != 1) {
		if (markedmines == minesAmount && markers == 0) {
			showmines = 1;
			system("cls");
			fieldPrint(fieldArr, Xsize, Ysize, showmines);
			position.X = 0;
			position.Y = Ysize + 1;
			printf("You win!!!\nYou win!!!\nYou win!!!\nYou win!!! ");
			timeend = time(NULL);
			timeplayedsec = timeend - timestart;
			while (timeplayedsec >= 60) {
				timeplayedsec -= 60;
				timeplayedmin++;
			}
			printf("\nTime played: %d minutes %d seconds ", timeplayedmin, timeplayedsec);
			gameover = 1;
			Sleep(3000);
		}
		position.X = 0;
		position.Y = Ysize + 1;
		printf("\b "); //удаляет значок '<' на старой позиции
		SetConsoleCursorPosition(hConsole, position);
		printf("Coords: X = %-3d Y = %-3d Markers: %d /*MarkedMines: %d */", x, y, markers, markedmines);
		printf("\nControl: 4 - left, 6 - right, 8 - up, 2 - down, 5 - open cell, 0 - mark cell");
		position.X = 2 * x + 1;                               //Установка координаты Х
		position.Y = y;										  //Установка координаты Y
		SetConsoleCursorPosition(hConsole, position);		  //Переход каретки по X Y координатам
		printf("<");
		xy = _getch();
		switch (xy) {			//Ввод
		case 50: y++; break;	// num 2 - down
		case 56: y--; break;	// num 8 - up
		case 54: x++; break;	// num 6 - left
		case 52: x--; break;	// num 4 - right
		case 53: {				// num 5 - open cell
			if (*(fieldArr + x + Xsize * y) != mine && *(fieldArr + x + Xsize * y) != markedmine) {
				*(fieldArr + x + Xsize * y) = uncheckedopen;
				int mines = checkNeighbors(fieldArr, Xsize, Ysize, y, x);
				printf("\b\b%d ", mines);
				if (mines == 0) {
					checkNulls(fieldArr, Xsize, Ysize, y, x);
				}
			}
			else { //если часть ниже исполняется - попал по мине
				showmines = 1;
				system("cls");
				fieldPrint(fieldArr, Xsize, Ysize, showmines);
				position.X = 0;                              
				position.Y = Ysize+1;
				printf("You lose!");
				timeend = time(NULL);
				timeplayedsec = timeend - timestart;
				while (timeplayedsec >= 60) {
					timeplayedsec -= 60;
					timeplayedmin++;
				}
				printf("\nTime played: %d minutes %d seconds ", timeplayedmin, timeplayedsec);
				gameover = 1; Sleep(3000);
			}
		} break;
		case 48: {				// num 7 - mark/unmark mine	
			if (*(fieldArr + x + Xsize * y) == mine) {					//если помечена мина - становится мина с меткой
				*(fieldArr + x + Xsize * y) = markedmine;
				markedmines++;
				markers--;
				printf("\b\b! ");
			}
			else if (*(fieldArr + x + Xsize * y) == markedmine) {		//если помечена метка с миной - становится миной 
				*(fieldArr + x + Xsize * y) = mine;
				markedmines--;
				markers++;
				printf("\b\b: ");
			}
			else if (*(fieldArr + x + Xsize * y) == empty ) {			//если помечена пустая - становится пустой с меткой
				*(fieldArr + x + Xsize * y) = markedempty;
				markers--;
				printf("\b\b! ");
			}
			else if (*(fieldArr + x + Xsize * y) == markedempty) {		//если помечена пустая c меткой - становится пустой
				*(fieldArr + x + Xsize * y) = empty;
				markers++;
				printf("\b\b: ");
			}
		}
		default: break;
		}
		if (x < 0) x = 0;						//не даёт "курсору" вылезти за поле
		else if (x >= Xsize) x = Xsize - 1;
		if (y < 0) y = 0;
		else if (y >= Ysize) y = Ysize - 1;
	}
}