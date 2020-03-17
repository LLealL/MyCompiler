#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "type.h"


int lookup(char string[]);

void ignoraBlank(char *c, int *linha, int *coluna, FILE *arq);

void getProxChar(char *c, int *linha, int *coluna, FILE *arq);

Ttoken TratarPalavra(char *c, int *linha, int *coluna, FILE *arq, char buffer[]);

Ttoken TratarNumero(char *c, int *linha, int*coluna, FILE *arq, char buffer[]);

Ttoken Scan(FILE *arq, int *linha, int *coluna);
