#include "VarStackType.h"

void criarStack(VarStack p);
void push(VarStack *p, TvarDecl variavel);
int hasVar(VarStack *p, char lexema[]);
int VarInEscopo(VarStack *p, char lexema[], int escopo);
void deleteEscopo(VarStack *p, int escopo);
int EscopoControl(int i); //teste
int getVarType(VarStack *p, char lexema[]);
