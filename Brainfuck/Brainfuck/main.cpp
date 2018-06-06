#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#include<iostream>
#include<string>
#include<thread>
using namespace std;

char program[100000];
char* loops[20];
int loopIndex = -1;
bool finished = false;

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

void brain_putchar(char *p) {
	__asm
	{
		mov eax, 4
		mov ebx, 1
		mov ecx, p
		mov edx, 1
		int 0x80
	}
}

void readCodeFromFile() {
	char * prog = program;
	FILE *f;
	char buff[4] = { '\0' };
	size_t bytesRead = 0;
	f = fopen("brain.txt", "r");

	if (f != NULL)
	{
		// read up to sizeof(buffer) bytes
		while ((bytesRead = fread(buff, 1, sizeof(buff), f)) > 0)
		{
			memcpy(prog, buff, bytesRead);
			memset(buff, '\0', 4);
			prog += 4;
		}
	}
	fclose(f);
	finished = true;
}

int main(int argc, char* argv[]) {
	char *mainTable = new char[100000]; // wynik programu
	memset(mainTable, 0, 100000);	// zerowanie tablicy w ktorej bedzie wynik
	memset(loops, 0, 20);
	memset(program, '\0', 100000); // zerowanie tablicy z kodem programu
	char *code = program; // wskaznik na poczatek odczytanego kodu
	char *p = mainTable; // wskaznik na poczatek tablicy ktora bedzie wynikiem
	thread t(readCodeFromFile);

	while (!finished) {
		while (code[0] != '\0') {
			char c = code[0];
			if (c == '>') {
				p = brain_mov_r(p);
			}
			else if (c == '<') {
				p = brain_mov_l(p);
			}
			else if (c == '+') {
				brain_add(p); //++(*p);
			}
			else if (c == '-') {
				brain_sub(p); //--(*p);
			}
			else if (c == '.') {
				putchar(*p);
			}
			else if (c == ',') {
				*p = getchar();
			}
			else if (c == '[') {
				loopIndex++;
				loops[loopIndex] = code;
			}
			else if (c == ']') {
				if (*p != 0) {
					code = loops[loopIndex];
				}
				else {
					loopIndex--;
				}
			}
			code++;
		}
	}

	printf("%s", "\n");
	//delete mainTable;
	t.join();
	system("pause");
	return 0;
}