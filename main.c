#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "type.h"
#include "Lexer.h"
#include "Parser.h"

int main(int Argc, char* Argv[]) {
	//char s[256];
	Ttoken t;
	int linha = 1;
	int coluna = 1;

	//printf("Digite nome do arquivo:\n");
	//scanf("%s", &s);

	
	//FILE *file = fopen(Argv[1], "r");
	FILE *file = fopen("file2.txt", "r");

	PROGRAM(file);
	/*
	while (1) {

		t = Scan(file,&linha,&coluna);
		//printf("token : %d , Lexema: %s \n", t.value, t.lexema);
		if (t.value == EOFF) {
			break;
		}
	}
	*/
	fclose(file);

	return 0;
}
