#include "Lexer.h"

#define tambuffer 512


int lookup(char string[]) {
	if (strcmp(string, "main") == 0) {
		return MAIN;
	}
	else if (strcmp(string, "if") == 0) {
		return IF;
	}
	else if (strcmp(string, "else") == 0) {
		return ELSE;
	}
	else if (strcmp(string, "while") == 0) {
		return WHILE;
	}
	else if (strcmp(string, "do") == 0) {
		return DO;
	}
	else if (strcmp(string, "for") == 0) {
		return FOR;
	}
	else if (strcmp(string, "int") == 0) {
		return INT;
	}
	else if (strcmp(string, "float") == 0) {
		return FLOAT;
	}
	else if (strcmp(string, "char") == 0) {
		return CHAR;
	}
	else {
		return ID;
	}
}

void ignoraBlank(char *c, int *linha, int *coluna, FILE *arq) {

	if (*c == '\n') {
		*coluna = 0;
		*linha = *linha + 1;
	}
	if (*c == '\t') {
		*coluna = *coluna + 4;
	}
	if (isspace(*c)) {
		*coluna = *coluna + 1;
	}
	while (isspace(*c) || *c == '\t' || *c == '\n') {
		*c = fgetc(arq);
		if (*c == '\n') {
			*coluna = 0;
			*linha = *linha + 1;
		}
		if (*c == '\t') {
			*coluna = *coluna + 4;
		}
		if (isspace(*c)) {
			*coluna = *coluna + 1;
		}
	}
}

void getProxChar(char *c, int *linha, int *coluna, FILE *arq) {
	*c = fgetc(arq);
	*coluna = *coluna + 1;
	/*
	if (*c == '\t') {
	*coluna = *coluna + 4;
	}
	if (*c == '\n') {
	*linha = *linha + 1;
	*coluna = 0;
	}*/
}

Ttoken TratarPalavra(char *c, int *linha, int *coluna, FILE *arq, char buffer[]) {
	int i = 0;
	int v;
	Ttoken t;

	while (isalnum(*c) || *c == '_') {
		if (i < tambuffer - 1) {
			buffer[i] = *c;
			*c = fgetc(arq);
			*coluna = *coluna + 1;
			i = i + 1;
		}
		else {
			break;
		}
	}
	buffer[i] = '\0';
	v = lookup(buffer);
	if (v == ID) {
		buffer[i] = '\0';
		strcpy(t.lexema, buffer);
		t.value = v;
		return t;
	}
	else {
		t.value = v;
		strcpy(t.lexema, "\0");
		return t;
	}
}

Ttoken TratarNumero(char *c, int *linha, int*coluna, FILE *arq, char buffer[]) {
	int i = 0;
	Ttoken t;

	while (isdigit(*c)) {
		if (i < tambuffer - 1) {
			buffer[i] = *c;
			*c = fgetc(arq);
			*coluna = *coluna + 1;
			i = i + 1;
		}
		else {

			break;
		}
	}
	if (*c == '.') {
		if (i < tambuffer - 1) {
			buffer[i] = *c;
			*c = fgetc(arq);
			*coluna = *coluna + 1;
			i = i + 1;
		}
		else {
			goto ErroEscritaFloat;
		}
		if (isdigit(*c)) {
			do {
				if (i < tambuffer - 1) {
					buffer[i] = *c;
					*c = fgetc(arq);
					*coluna = *coluna + 1;
					i = i + 1;
				}
				else {
					break;
				}
			} while (isdigit(*c));
			buffer[i] = '\0';
			strcpy(t.lexema, buffer);
			t.value = DECIMAL;
			return t;
		}
		else {
			goto ErroEscritaFloat;
		}
	}
	else {
		buffer[i] = '\0';
		strcpy(t.lexema, buffer);
		t.value = INTEIRO;
		return t;
	}


ErroEscritaFloat:
	printf("ERRO na linha %d, coluna %d, ultimo token lido t: float mal formatado\n", *linha, *coluna);
	t.value = ERROR;
	//getProxChar(&c, &linha, &coluna, arq);
	return t;
}

Ttoken Scan(FILE *arq, int *linha, int *coluna) {
	static Ttoken t;
	char buffer[tambuffer] = "\0";
	static char carDaVez = ' ';

Inicio:
	t.value = 0;
	t.lexema[0] = '\0';
	ignoraBlank(&carDaVez, linha, coluna, arq);
	if (isalpha(carDaVez) || carDaVez == '_') {
		return TratarPalavra(&carDaVez, linha, coluna, arq, buffer);
	}
	else if (isdigit(carDaVez) || carDaVez == '.') {
		t = TratarNumero(&carDaVez, linha, coluna, arq, buffer);
		if (t.value == ERROR) {
			goto Inicio;
		}
		return t;
	}
	else if (carDaVez == '/') {
		strcpy(t.lexema, "\0");
		getProxChar(&carDaVez, linha, coluna, arq);
		if (carDaVez == '/') {
			while (carDaVez != '\n' && !feof(arq)) {
				getProxChar(&carDaVez, linha, coluna, arq);
			}
			if (carDaVez == '\n') {
				*coluna = 0;
				*linha = *linha + 1;
				getProxChar(&carDaVez, linha, coluna, arq);
				goto Inicio;
			}
			else {
				t.value = EOFF;
				return t;
			}
		}
		else if (carDaVez == '*') {
			getProxChar(&carDaVez, linha, coluna, arq);
		Comentario:
			while (carDaVez != '*') {
				getProxChar(&carDaVez, linha, coluna, arq);
				if (feof(arq)) {
					goto ErroComentEOF;
				}
				if (carDaVez == '\n') {
					*coluna = 0;
					*linha = *linha + 1;
				}
				if (carDaVez == '\t') {
					*coluna = *coluna + 4;
				}
			}
			if (carDaVez == '*') {
				carDaVez = fgetc(arq);
				*coluna = *coluna + 1;
				while (carDaVez == '*') {
					getProxChar(&carDaVez, linha, coluna, arq);
				}
				if (carDaVez == '/') {
					getProxChar(&carDaVez, linha, coluna, arq);
					goto Inicio;
				}
				else if (feof(arq)) {
					goto ErroComentEOF;
				}
				else {
					goto Comentario;
				}
			}
			else {
				goto ErroComentEOF;
			}
		}
		else {
			t.value = DIV;
			return t;
		}
	}
	else if (carDaVez == '\'') {
		//buffer[0] = carDaVez;
		carDaVez = fgetc(arq);
		*coluna = *coluna + 1;
		if (isalnum(carDaVez)) {
			buffer[0] = carDaVez;
			getProxChar(&carDaVez, linha, coluna, arq);
			if (carDaVez == '\'') {
				//buffer[2] = carDaVez;
				buffer[1] = '\0';
				strcpy(t.lexema, buffer);
				t.value = VALCHAR;
				getProxChar(&carDaVez, linha, coluna, arq);
				return t;
			}
			else {
				goto ErroChar;
			}
		}
		else {
			goto ErroChar;
		}

	}
	else if (carDaVez == '<') {
		getProxChar(&carDaVez, linha, coluna, arq);
		if (carDaVez == '=') {
			getProxChar(&carDaVez, linha, coluna, arq);
			t.value = MENORIGUAL;
		}
		else {
			t.value = MENORQ;
		}
		return t;

	}
	else if (carDaVez == '>') {
		getProxChar(&carDaVez, linha, coluna, arq);
		if (carDaVez == '=') {
			getProxChar(&carDaVez, linha, coluna, arq);
			t.value = MAIORIGUAL;
		}
		else {
			t.value = MAIORQ;
		}
		return t;
	}
	else if (carDaVez == '!') {
		getProxChar(&carDaVez, linha, coluna, arq);
		if (carDaVez == '=') {
			getProxChar(&carDaVez, linha, coluna, arq);
			t.value = DIFERENTE;
			return t;
		}
		else {
			goto ErroDif;
		}
	}
	else if (carDaVez == '+') {
		getProxChar(&carDaVez, linha, coluna, arq);
		t.value = ADD;
		return t;
	}
	else if (carDaVez == '-') {
		getProxChar(&carDaVez, linha, coluna, arq);
		t.value = SUB;
		return t;
	}
	else if (carDaVez == '*') {
		getProxChar(&carDaVez, linha, coluna, arq);
		t.value = MUL;
		return t;
	}
	else if (carDaVez == '=') {
		getProxChar(&carDaVez, linha, coluna, arq);
		if (carDaVez == '=') {
			getProxChar(&carDaVez, linha, coluna, arq);
			t.value = IGUALIGUAL;
		}
		else {
			t.value = ATRIB;
		}
		return t;
	}
	else if (carDaVez == '(') {
		getProxChar(&carDaVez, linha, coluna, arq);
		t.value = ABREPARENTESES;
		return t;
	}
	else if (carDaVez == ')') {
		getProxChar(&carDaVez, linha, coluna, arq);
		t.value = FECHAPARENTESES;
		return t;
	}
	else if (carDaVez == '{') {
		getProxChar(&carDaVez, linha, coluna, arq);
		t.value = ABREBLOCO;
		return t;
	}
	else if (carDaVez == '}') {
		getProxChar(&carDaVez, linha, coluna, arq);
		t.value = FECHABLOCO;
		return t;
	}
	else if (carDaVez == ',') {
		getProxChar(&carDaVez, linha, coluna, arq);
		t.value = VIRGULA;
		return t;
	}
	else if (carDaVez == ';') {
		getProxChar(&carDaVez, linha, coluna, arq);
		t.value = PV;
		return t;
	}
	else if (feof(arq)) {
		t.value = EOFF;
		return t;
	}
	else {
		printf("ERRO na linha %d, coluna %d, ultimo token lido t: Caractere nao reconhecido\n", *linha, *coluna);
		getProxChar(&carDaVez, linha, coluna, arq);
		goto Inicio;
	}

ErroChar:
	printf("ERRO na linha %d, coluna %d, ultimo token lido t: Char mal formatado\n", *linha, *coluna);
	getProxChar(&carDaVez, linha, coluna, arq);
	while (carDaVez != '\'') {
		getProxChar(&carDaVez, linha, coluna, arq);
	}
	getProxChar(&carDaVez, linha, coluna, arq);
	goto Inicio;
ErroDif:
	printf("ERRO na linha %d, coluna %d, ultimo token lido t: Diferença mal formatada\n", *linha, *coluna);
	getProxChar(&carDaVez, linha, coluna, arq);
	goto Inicio;
ErroComentEOF:
	printf("ERRO na linha %d, coluna %d, ultimo token lido t: Comentario em aberto\n", *linha, *coluna);
	goto Inicio;


}
