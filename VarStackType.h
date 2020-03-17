#ifndef TypeStack
#define TypeStack
#include <stdio.h>
#include <stdlib.h>
#include "type.h"


typedef struct varDeclarada {
	int tipo;
	char lexema[tambuffer];
	int escopo;
}TvarDecl;

typedef struct VarStackNode {
	TvarDecl variavel;
	struct VarStackNode *proximo;
}TStackNode;

typedef TStackNode * VarStack;

#endif