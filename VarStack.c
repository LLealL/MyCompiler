#include "VarStack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void criarStack(VarStack p) {
	p = NULL;
}
void push(VarStack *p, TvarDecl variavel) {
	TStackNode *novo;
	novo = (TStackNode *)malloc(sizeof(TStackNode));
	novo->proximo = *p;
	novo->variavel = variavel;
	*p = novo;
}
int hasVar(VarStack *p, char lexema[]) {
	VarStack v;
	v = *p;
	if (*p != NULL) {
		while (v!= NULL) {
			if (strcmp(v->variavel.lexema, lexema) == 0) {
				return 1;
			}
			v = v->proximo;
		}
	}
	return 0;

}
int VarInEscopo(VarStack *p, char lexema[], int escopo) {
	VarStack v;

	if (*p!=NULL) {
		v = *p;
		while (v->proximo != NULL && escopo == v->variavel.escopo) {
			if (strcmp(v->variavel.lexema, lexema) == 0) {
				return 1;
			}
			v = v->proximo;
		}
	}
	return 0;
}
void deleteEscopo(VarStack *p, int escopo) {
	VarStack v;

	v = *p;
	if (*p != NULL) {
		while (v->proximo != NULL && escopo == v->variavel.escopo) {
			v = (*p)->proximo;	
			free(*p);
			*p = v;
		}
	}
}
int EscopoControl(int i) { //teste
	static int cont = 0;
	cont = cont + i;
	return cont;
}

int getVarType(VarStack *p, char lexema[]) {
	VarStack v;

	v = *p;
	if (*p != NULL) {
		while (v != NULL) {
			if (strcmp(v->variavel.lexema, lexema) == 0) {
				return v->variavel.tipo;
			}
			v = v->proximo;
		}
	}
	return TYPEVOID;
}