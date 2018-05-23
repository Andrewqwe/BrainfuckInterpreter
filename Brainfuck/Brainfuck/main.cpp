#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#include<iostream>
#include<string>
#include<thread>
#include "main.h"
using namespace std;

char program[100000];
char* loops[20];
int loopIndex = -1;

void readCodeFromFile(string file) {
	char * prog = program;
	FILE *f;
	f = fopen("brain.txt", "r");
	char buff[4] = { '\0' };

	for (int i = 0; i < file.length(); i++) {
		cout << "setting " << file.at(i) << " to program[" << i << "]" << endl;
		program[i] = file.at(i);
	}
	fclose(f);
}

char* brain_mov_r(char * p) {
	__asm
	{
		mov eax, p
		add eax, 0x1
	}
}

char* brain_mov_l(char * p) {
	__asm
	{
		mov eax, p
		sub eax, 0x1
	}
}

char* brain_add(char * p) {
	__asm
	{
		push eax
		mov eax, [p]
		inc dword ptr [eax]
		pop eax
	}
}

char* brain_sub(char * p) {
	__asm
	{
		push eax
		mov eax, [p]
		dec dword ptr[eax]
		pop eax
	}
}

int main(int argc, char* argv[]) {
	char *mainTable = new char[100000]; // wynik programu
	memset(mainTable, 0, 100000);	// zerowanie tablicy w ktorej bedzie wynik
	memset(loops, 0, 20);
	memset(program, '\0', 100000); // zerowanie tablicy z kodem programu
	mainTable[0] = 97;
	mainTable[1] = 98;
	mainTable[2] = 99;
	printf("%s", "zaczynam wyczytwac program \n");
	thread t(readCodeFromFile, ">>>>++++++++++[<++++++++++>]");
	printf("%s", "zaczynam kompilowac \n");
	system("pause");
	char *code = program; // wskaznik na poczatek odczytanego kodu
	char *p = mainTable; // wskaznik na poczatek tablicy ktora bedzie wynikiem

	while (code[0] != '\0') {
		char c = code[0];
		if (c == '>') {
			p = brain_mov_r(p);
			cout << '>';
		}
		else if (c == '<') {
			p = brain_mov_l(p);
			cout << '<';
		}
		else if (c == '+') {
			brain_add(p); //++(*p);
			cout << '+';
		}
		else if (c == '-') {
			brain_sub(p); //--(*p);
			cout << '-';
		}
		else if (c == '.') {
			putchar(*p);
			cout << '.';
		}
		else if (c == ',') {
			*p = getchar();
			cout << ',';
		}
		else if (c == '[') {
			cout << '[';
			loopIndex++;
			loops[loopIndex] = code;
		}
		else if (c == ']') {
			cout << ']';
			brain_sub(p);
			if (*p != 0) {
				code = loops[loopIndex];
			}
			else {
				loopIndex--;
			}
		}
		code++;
	}

	cout << endl << ((int)mainTable[0]) << " ";
	cout << endl << ((int)mainTable[1]) << " ";
	cout << endl << ((int)mainTable[2]) << " ";
	cout << endl << ((int)mainTable[3]) << " ";
	cout << endl << ((int)mainTable[4]) << " ";
	cout << endl << mainTable << endl;


	//delete mainTable;
	t.join();
	system("pause");
	return 0;
}