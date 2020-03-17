#include <stdio.h>
#include <stdlib.h>
#include "type.h"
#include "Lexer.h"
#include "VarStack.h"


Tsemantic EXP_REL(FILE *arq, Ttoken *lookahead, int *linha, int *coluna,VarStack *p);
void ITERACAO(FILE *arq, Ttoken *lookahead, int *linha, int *coluna,VarStack *p);
Tsemantic EXP(FILE *arq, Ttoken *lookahead, int *linha, int *coluna,VarStack *p);
Tsemantic EXP_ARIT(FILE *arq, Ttoken *lookahead, int *linha, int *coluna,VarStack *p);
Tsemantic TERMO(FILE *arq, Ttoken *lookahead, int *linha, int *coluna,VarStack *p);
Tsemantic FATOR(FILE *arq, Ttoken *lookahead, int *linha, int *coluna,VarStack *p);
void ATRIBUICAO(FILE *arq, Ttoken *lookahead, int *linha, int *coluna,VarStack *p);
void CMDBASICO(FILE *arq, Ttoken *lookahead, int *linha, int *coluna,VarStack *p);
void DECLVAR(FILE *arq, Ttoken *lookahead, int *linha, int *coluna, VarStack *p);
void CMD(FILE *arq, Ttoken *lookahead, int *linha, int *coluna,VarStack *p);
void BLOCO(FILE *arq, Ttoken *lookahead, int *linha, int *coluna,VarStack *p);
void getProxToken(FILE *arq, Ttoken *t, int *linha, int *coluna);
void PROGRAM(FILE *arq);
