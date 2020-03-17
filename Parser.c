#include "Parser.h"
#include "VarStack.h"
#include "CodeGenerator.h"
/*
	1-Controle de Escopo pronto (x)
	2-Regras Semanticas Prontas ( )
		2.1- Checagem de variaveis declaradas (x)
		2.2- Checagem de tipos ( )
	3- Gerador de código ( )

*/


void IFSTMT(FILE *arq, Ttoken *lookahead, int *linha, int *coluna,VarStack *p) {
	IFlabel label;
	Tsemantic condResult;
	if (lookahead->value == IF) {
		label = newIFlabel();
		*lookahead = Scan(arq, linha, coluna);
		if (lookahead->value == ABREPARENTESES) {
			*lookahead = Scan(arq, linha, coluna);
			condResult=EXP_REL(arq, lookahead, linha, coluna,p);
			if (lookahead->value == FECHAPARENTESES) {
				//testa expressao 
				printf("if %s==0 goto %s \n", condResult.varTemp, label.notIF);
				*lookahead = Scan(arq, linha, coluna);
				CMD(arq, lookahead, linha, coluna,p);
				printf("goto %s\n", label.end);
				printf("%s :\n", label.notIF);
				if (lookahead->value == ELSE) {
					*lookahead = Scan(arq, linha, coluna);
					CMD(arq, lookahead, linha, coluna,p);
				}
				printf("%s :\n", label.end);
				return;
			}
			else {
				printf("ERRO na linha %d, coluna %d, ultimo token lido t: \")\" nao encontrado \n", *linha, *coluna);
				//missing )
			}
		}
		else {
			printf("ERRO na linha %d, coluna %d, ultimo token lido t: \"(\" nao encontrado \n", *linha, *coluna);
			//missing (
		}
	}
	else {
		printf("ERRO na linha %d, coluna %d, ultimo token lido t: \"if\" nao encontrado \n", *linha, *coluna);
		//missing IF
	}
}

Tsemantic EXP_REL(FILE *arq, Ttoken *lookahead, int *linha, int *coluna,VarStack *p) {
	Tsemantic r1,r2,varTemp,varReturn;
	TopRel opRel;

	varReturn = newVarTemp();
	r1=EXP_ARIT(arq, lookahead, linha, coluna,p);
	if (lookahead->value == DIFERENTE || lookahead->value == IGUALIGUAL || lookahead->value == MENORIGUAL || lookahead->value == MAIORIGUAL || lookahead->value == MENORQ || lookahead->value == MAIORQ) {
		opRel.codOp = lookahead->value;
		if (opRel.codOp == DIFERENTE) {
			strcpy(opRel.string, "!=");
		}
		else if (opRel.codOp == IGUALIGUAL) {
			strcpy(opRel.string, "==");
		}
		else if (opRel.codOp == MENORIGUAL) {
			strcpy(opRel.string, "<=");
		}
		else if (opRel.codOp == MAIORIGUAL) {
			strcpy(opRel.string, ">=");
		}
		else if (opRel.codOp == MENORQ) {
			strcpy(opRel.string, "<\0");
		}
		else{
			strcpy(opRel.string, ">\0");
		}

		*lookahead = Scan(arq, linha, coluna);
		r2=EXP_ARIT(arq, lookahead, linha, coluna,p);
		if (r1.type!=r2.type) {
			if ((r1.type == CHAR || r2.type == CHAR)&&r1.type!=TYPEVOID&&r2.type!=TYPEVOID) {
				printf("ERRO na linha %d, coluna %d, ultimo token lido t: tipos char incompativel com outro tipo em operacao relacional\n", *linha, *coluna);
			}
			else {
				if (r1.type == TYPEVOID || r2.type == TYPEVOID) {
					printf("ERRO na linha %d, coluna %d, ultimo token lido t: variavel nao declarada sendo usada em expressao relacional\n", *linha, *coluna);
				}
				else {
					if (r1.type == FLOAT && r2.type==INT) {
						varTemp = newVarTemp();
						varTemp.type = FLOAT;
						printf("%s = FLOAT %s\n", varTemp.varTemp, r2.varTemp);
				//		varReturn = newVarTemp();
						printf("%s =  %s %s %s\n", varReturn.varTemp, r1.varTemp, opRel.string, varTemp.varTemp);
				


					}
					else { //r1=int && r2==Float
						varTemp = newVarTemp();
						varTemp.type = FLOAT;
						printf("%s = FLOAT %s\n", varTemp.varTemp, r1.varTemp);
					//	varReturn = newVarTemp();
						printf("%s =  %s %s %s\n", varReturn.varTemp, varTemp.varTemp, opRel.string, r2.varTemp);
					}
				}
			}
		}
		else {
			if (r1.type != TYPEVOID) {
		//		varReturn = newVarTemp();
				printf("%s =  %s %s %s\n", varReturn.varTemp, r1.varTemp, opRel.string, r2.varTemp);
			}
			else {
				printf("ERRO na linha %d, coluna %d, ultimo token lido t: variaveis não declaradas em expressão relacional \n", *linha, *coluna);
			}
		}
	}
	else {
		printf("ERRO na linha %d, coluna %d, ultimo token lido t: Operacao Relacional nao encontrada \n", *linha, *coluna);
		//missing op relacional
	}
	return varReturn;

}

void ITERACAO(FILE *arq, Ttoken *lookahead, int *linha, int *coluna,VarStack *p) {
	Whilelabel label;
	Tsemantic condResult;

	label = newWhileLabel();
	if (lookahead->value == WHILE) {
		*lookahead = Scan(arq, linha, coluna);
		if (lookahead->value == ABREPARENTESES) {
			*lookahead = Scan(arq, linha, coluna);
			printf("%s :\n", label.begin);
			condResult= EXP_REL(arq, lookahead, linha, coluna,p);
			if (lookahead->value == FECHAPARENTESES) {
				*lookahead = Scan(arq, linha, coluna);
				printf("if %s == 0 goto %s\n", condResult.varTemp, label.end);
				CMD(arq, lookahead, linha, coluna,p);
				printf("goto %s\n", label.begin);
				printf("%s :\n", label.end);
				return;
			}
			else {
				printf("ERRO na linha %d, coluna %d, ultimo token lido t: \")\" nao encontrado \n", *linha, *coluna);
				//missing )
			}
		}
		else {
			printf("ERRO na linha %d, coluna %d, ultimo token lido t: \"(\" nao encontrado \n", *linha, *coluna);
			// missing (
		}
	}
	else if (lookahead->value == DO) {
		*lookahead = Scan(arq, linha, coluna);
		printf("%s :\n", label.begin);
		CMD(arq, lookahead, linha, coluna,p);
		if (lookahead->value == WHILE) {
			*lookahead = Scan(arq, linha, coluna);
			if (lookahead->value == ABREPARENTESES) {
				*lookahead = Scan(arq, linha, coluna);
				condResult=EXP_REL(arq, lookahead, linha, coluna,p);
				if (lookahead->value == FECHAPARENTESES) {
					*lookahead = Scan(arq, linha, coluna);
					if (lookahead->value == PV) {
						*lookahead = Scan(arq, linha, coluna);
						//testa expressao
						printf("if %s!=0 goto %s \n", condResult.varTemp, label.begin);
						printf("%s :\n", label.end);
						return;
					}
					else {
						printf("ERRO na linha %d, coluna %d, ultimo token lido t: \";\" nao encontrado \n", *linha, *coluna);
						//missing ;
					}
				}
				else {
					printf("ERRO na linha %d, coluna %d, ultimo token lido t: \")\" nao encontrado \n", *linha, *coluna);
					//missing )
				}
			}
			else {
				printf("ERRO na linha %d, coluna %d, ultimo token lido t: \"(\" nao encontrado \n", *linha, *coluna);
				//missing (
			}
		}
		else {
			printf("ERRO na linha %d, coluna %d, ultimo token lido t: \"while\" nao encontrado \n", *linha, *coluna);
			//missing while
		}
	}
	else {
		printf("ERRO na linha %d, coluna %d, ultimo token lido t: iteracao nao encontrada \n", *linha, *coluna);
		//missing iteracao 
	}
} //aplicar controle de escopo

Tsemantic EXP(FILE *arq, Ttoken *lookahead, int *linha, int *coluna,VarStack *p) {
	// <exp> -> + <termo><exp> | - <termo><exp> | "vazio" 
	Tsemantic t1,t2,varTemp,varTemp2;

	char op;
	t1.type = TYPEVOID; // tipo para retornar "nulo"
	t2.type = TYPEVOID; // tipo para retornar "nulo"
	
	if (lookahead->value == ADD || lookahead->value == SUB) {
		if (lookahead->value == ADD) {
			op = '+';
		}
		else {
			op = '-';
		}
		*lookahead = Scan(arq, linha, coluna);
		t1=TERMO(arq, lookahead, linha, coluna,p);
		t2=EXP(arq, lookahead, linha, coluna,p);
		if (t1.type != t2.type) {
			if ((t1.type == CHAR || t2.type == CHAR)&&t2.type!=TYPEVOID) {
				if (t1.type == TYPEVOID) {
					printf("ERRO na linha %d, coluna %d, ultimo token lido t: variavel nao declarada sendo usada em operacao aritmetica \n", *linha, *coluna);
				}
				else {
					printf("ERRO na linha %d, coluna %d, ultimo token lido t: tipos invalidos na operacao aritmetica \n", *linha, *coluna);
				}
			}
			else {
				if (t1.type == FLOAT && t2.type == INT) {
					varTemp = newVarTemp();
					varTemp.type = FLOAT;					
					printf("%s = FLOAT %s\n",varTemp.varTemp, t2.varTemp);
					varTemp.op = t2.op;
					varTemp2 = newVarTemp();
					varTemp2.type = varTemp.type;
					printf("%s = %s %c %s\n",varTemp2.varTemp,t1.varTemp,varTemp.op,varTemp.varTemp);
					strcpy(t1.varTemp, varTemp2.varTemp);
				}
				else if (t1.type == INT && t2.type == FLOAT) {
					varTemp = newVarTemp();
					varTemp.type = FLOAT;
					printf("%s = FLOAT %s\n", varTemp.varTemp, t1.varTemp);
					varTemp2 = newVarTemp();	
					varTemp2.type = varTemp.type;
					printf("%s = %s %c %s\n", varTemp2.varTemp, varTemp.varTemp, t2.op, t2.varTemp);
					strcpy(t1.varTemp, varTemp2.varTemp);
				}
			}
		}
		else {
			if (t1.type == TYPEVOID) {
				printf("ERRO na linha %d, coluna %d, ultimo token lido t: variavel nao declarada sendo usada em operacao aritmetica \n", *linha, *coluna);
			}
			else {
				varTemp = newVarTemp();
				varTemp.type = t1.type;
				printf("%s = %s %c %s\n", varTemp.varTemp, t1.varTemp,t2.op, t2.varTemp);
				strcpy(t1.varTemp, varTemp.varTemp);
			}
			
		//	t1.type = t2.type;
		}
		t1.op = op;
	}
	return t1;
}

Tsemantic EXP_ARIT(FILE *arq, Ttoken *lookahead, int *linha, int *coluna,VarStack *p) {
	//BNFE => <exp_arit> ::= <Termo> { {+|-} <Termo>}*
	//gramatica pedida: <exp_arit> -> <termo><exp> 
	Tsemantic t1,t2,varTemp;

	t1=TERMO(arq, lookahead, linha, coluna,p);
	t2=EXP(arq, lookahead, linha, coluna,p);

	if (t1.type != t2.type) {
		if ((t1.type == CHAR || t2.type == CHAR)&&t2.type!=TYPEVOID) {
			if (t1.type == TYPEVOID) {
				printf("ERRO na linha %d, coluna %d, ultimo token lido t: variavel nao declarada sendo usada em operacao aritmetica \n", *linha, *coluna);
			}
			else {
				printf("ERRO na linha %d, coluna %d, ultimo token lido t: tipos invalidos na operacao aritmetica \n", *linha, *coluna);
			}
		}
		else {
			if (t1.type == FLOAT && t2.type == INT){
				varTemp = newVarTemp();
				varTemp.type = t1.type;
				printf("%s = FLOAT %s\n", varTemp.varTemp, t2.varTemp);
				t2.type = varTemp.type;
				strcpy(t2.varTemp, varTemp.varTemp);
				varTemp = newVarTemp();
				varTemp.type = t1.type;
				printf("%s = %s %c %s\n", varTemp.varTemp, t1.varTemp, t2.op, t2.varTemp);
				strcpy(t1.varTemp, varTemp.varTemp);
			}
			else if(t1.type==INT &&t2.type==FLOAT)
			{
				varTemp = newVarTemp();
				varTemp.type = t2.type;
				printf("%s = FLOAT %s\n", varTemp.varTemp, t1.varTemp);
				strcpy(t1.varTemp, varTemp.varTemp);
				varTemp = newVarTemp();
				printf("%s = %s %c %s\n", varTemp.varTemp, t1.varTemp, t2.op, t2.varTemp);
				strcpy(t1.varTemp, varTemp.varTemp);
			}
			else {	
				// verificar retorno t2=TYPEVOID
			}
		}
	}
	else {
		if (t1.type != TYPEVOID) {
			varTemp = newVarTemp();
			varTemp.type = t1.type;
			printf("%s = %s %c %s\n", varTemp.varTemp, t1.varTemp, t2.op, t2.varTemp);
			strcpy(t1.varTemp, varTemp.varTemp);
		}
		else {
			printf("ERRO na linha %d, coluna %d, ultimo token lido t: variavel nao declarada sendo usada em operacao aritmetica \n", *linha, *coluna);
		}
	}
	return t1;

	/*	//metodo iterativo para não usar pilha de recursão.
	while (lookahead->value == ADD || lookahead->value == SUB) {
	*lookahead = Scan(arq, linha, coluna);
	TERMO(arq, lookahead, linha, coluna);
	}
	return;
	*/

}

Tsemantic TERMO(FILE *arq, Ttoken *lookahead, int *linha, int *coluna,VarStack *p) {
	//BNFE => <Termo> ::= <Fator> { {*|/} <Fator> }*
	Tsemantic r1,r2,varTemp;
	char op;
	r1=FATOR(arq, lookahead, linha, coluna,p);

	while (lookahead->value == MUL || lookahead->value == DIV) {
		if (lookahead->value == MUL) {
			op = '*';
		}
		else {
			op = '/';
		}
		*lookahead = Scan(arq, linha, coluna);
		r2=FATOR(arq, lookahead, linha, coluna,p);
		if (r1.type==r2.type) {
			if (op == '/' && r2.type==INT) {
				varTemp = newVarTemp();
				varTemp.type = r2.type;
				printf("%s = %s %c %s \n", varTemp.varTemp, r1.varTemp, op ,r2.varTemp);
				r1.type = FLOAT;
				strcpy(r1.varTemp, varTemp.varTemp);
				varTemp = newVarTemp();
				varTemp.type = r1.type;
				printf("%s = FLOAT %s\n", varTemp.varTemp, r1.varTemp);
				strcpy(r1.varTemp, varTemp.varTemp);
			}
			else {
				varTemp = newVarTemp();
				varTemp.type = r2.type;
				printf("%s = %s %c %s\n", varTemp.varTemp, r1.varTemp, op, r2.varTemp);
				r1.type = r2.type;
				strcpy(r1.varTemp, varTemp.varTemp);
			}
		}
		else if (r1.type == CHAR || r2.type == CHAR) {
			if (r2.type == TYPEVOID) {
				printf("ERRO na linha %d, coluna %d, ultimo token lido t: variavel nao declarada sendo usada em operacao aritmetica \n", *linha, *coluna);
			}
			else {
				printf("ERRO na linha %d, coluna %d, ultimo token lido t: tipos invalidos na operacao aritmetica \n", *linha, *coluna);
			}
		}
		else {
			if (r1.type ==FLOAT && r2.type == INT) {
				varTemp = newVarTemp();
				varTemp.type = FLOAT;
				printf("%s = FLOAT %s\n", varTemp.varTemp, r2.varTemp);
				//cast r2 to FLOAT
				r2.type = varTemp.type;
				strcpy(r2.varTemp, varTemp.varTemp);
				varTemp = newVarTemp();
				printf("%s = %s %c %s\n", varTemp.varTemp, r1.varTemp, op, r2.varTemp);

				r1.type = FLOAT;
				strcpy(r1.varTemp, varTemp.varTemp);
			}
			else if (r1.type == INT && r2.type == FLOAT) {
				varTemp = newVarTemp();
				varTemp.type = FLOAT;
				printf("%s = FLOAT %s\n", varTemp.varTemp, r1.varTemp);
				//cast r1 to FLOAT
				strcpy(r1.varTemp, varTemp.varTemp);
				varTemp = newVarTemp();
				printf("%s = %s %c %s\n", varTemp.varTemp, r1.varTemp, op, r2.varTemp);
				r1.type = FLOAT;//
				strcpy(r1.varTemp, varTemp.varTemp);
			}
		}
	}
	return r1;
}

Tsemantic FATOR(FILE *arq, Ttoken *lookahead, int *linha, int *coluna,VarStack *p) {
	//BNFE => <Fator> ::= <id> | <float> | <inteiro> | <char> | "(" <exp_arit> ")" 
	char caractere[4];
	Tsemantic t1;
	if (lookahead->value == ABREPARENTESES) {
		*lookahead = Scan(arq, linha, coluna);
		t1=EXP_ARIT(arq, lookahead, linha, coluna,p);
		if (lookahead->value == FECHAPARENTESES) {
			*lookahead = Scan(arq, linha, coluna);
		}
		else {
			printf("ERRO na linha %d, coluna %d, ultimo token lido t: \")\" nao encontrado \n", *linha, *coluna);
			//ERRO MISSING )
		}
		return t1;
	}
	else if (lookahead->value == ID) {
		if (VarInEscopo(p, lookahead->lexema, EscopoControl(0))==1) {
			t1.type = getVarType(p, lookahead->lexema);
			strcpy(t1.varTemp,  lookahead->lexema);
		//	printf("\n");
			*lookahead = Scan(arq, linha, coluna);
			return t1;
		}
		else if(hasVar(p,lookahead->lexema)==1) {
			t1.type = getVarType(p, lookahead->lexema);
			strcpy(t1.varTemp, lookahead->lexema);
		//	printf("\n");
			*lookahead = Scan(arq, linha, coluna);
			return t1;
		}
		else {
			printf("ERRO na linha %d, coluna %d, ultimo token lido t: Variavel nao declarada!\n",*linha,*coluna);
			t1.type = TYPEVOID;
		}
		*lookahead = Scan(arq, linha, coluna);
		
			//variavel já declarada?

		return t1;
	}
	else if (lookahead->value == INTEIRO || lookahead->value == DECIMAL || lookahead->value == VALCHAR) {
		if (lookahead->value == INTEIRO) {
			t1.type = INT;
			strcpy(t1.varTemp, lookahead->lexema);
		}
		else if (lookahead->value == DECIMAL) {
			t1.type = FLOAT;
			strcpy(t1.varTemp, lookahead->lexema);
		}
		else {
			t1.type = CHAR;
			caractere[0] = '\'';
			caractere[2] = '\'';
			caractere[1] = lookahead->lexema[0];
			caractere[3] = '\0';
			strcpy(t1.varTemp, caractere);
		}
	//	strcpy(t.varTemp, lookahead->lexema);
		*lookahead = Scan(arq, linha, coluna);
		return t1;
	}
	else {
		printf("ERRO na linha %d, coluna %d, ultimo token lido t: Expressao aritmetica invalida \n", *linha, *coluna);
		t1.type = TYPEVOID;
		strcpy(t1.varTemp,"\0");
		t1.op = '\0';

		return t1;
		//ERRO EXP ARIT INVALIDA
	}

}

void ATRIBUICAO(FILE *arq, Ttoken *lookahead, int *linha, int *coluna,VarStack *p) {
	Tsemantic r1, tipoDestino, varTemp;
	tipoDestino.type = TYPEVOID;
	strcpy(tipoDestino.varTemp, "\0");
	if (lookahead->value == ID) {
		if (VarInEscopo(p, lookahead->lexema, EscopoControl(0))) {
			tipoDestino.type = getVarType(p, lookahead->lexema);
			strcpy(tipoDestino.varTemp, lookahead->lexema);
		}
		else if (hasVar(p, lookahead->lexema)) {
			tipoDestino.type = getVarType(p, lookahead->lexema);
			strcpy(tipoDestino.varTemp, lookahead->lexema);
		}
		else {
			printf("ERRO na linha %d, coluna %d, ultimo token lido t: variavel nao declarada! \n", *linha, *coluna);
		}
		*lookahead = Scan(arq, linha, coluna);
		if (lookahead->value == ATRIB) {
			*lookahead = Scan(arq, linha, coluna);
			r1=EXP_ARIT(arq, lookahead, linha, coluna,p);
			if (r1.type != tipoDestino.type) {
				if((tipoDestino.type==CHAR || r1.type==CHAR)&& tipoDestino.type!=TYPEVOID){
					printf("ERRO na linha %d, coluna %d, ultimo token lido t: Atribuicao de Tipos Invalidos! \n", *linha, *coluna);
				}else if (tipoDestino.type == INT && r1.type == FLOAT) {
					printf("ERRO na linha %d, coluna %d, ultimo token lido t: conversao de tipo invalida INT recebendo FLOAT! \n", *linha, *coluna);
				}
				else if (tipoDestino.type == FLOAT&&r1.type==INT) {
					//cast Float r1
					varTemp = newVarTemp();
					varTemp.type = tipoDestino.type;
					printf("%s = FLOAT %s\n", varTemp.varTemp, r1.varTemp);
					r1.type = varTemp.type;
					strcpy(r1.varTemp, varTemp.varTemp);
					printf("%s = %s \n", tipoDestino.varTemp, r1.varTemp);
				}
			}
			else {
				printf("%s = %s \n", tipoDestino.varTemp, r1.varTemp);
			//printf("tipos ok\n");
			}
			if (lookahead->value == PV) {
				*lookahead = Scan(arq, linha, coluna);
				return;
			}
			else {
				printf("ERRO na linha %d, coluna %d, ultimo token lido t: \";\" nao encontrado \n", *linha, *coluna);
				//erro missing  ;
			}
		}
		else {
			printf("ERRO na linha %d, coluna %d, ultimo token lido t: comando de atribuicao nao encontrado \n", *linha, *coluna);
			//erro missing =
		}
	}
	else {
		printf("ERRO na linha %d, coluna %d, ultimo token lido t: identificador nao encontrado \n", *linha, *coluna);
		//erro missing ID
	}
}

void CMDBASICO(FILE *arq, Ttoken *lookahead, int *linha, int *coluna,VarStack *p) {
	if (lookahead->value == ID) {
		ATRIBUICAO(arq, lookahead, linha, coluna,p);
		return;
	}
	else if (lookahead->value == ABREBLOCO) {
		BLOCO(arq, lookahead, linha, coluna,p);
		return;
	}
}

void DECLVAR(FILE *arq, Ttoken *lookahead, int *linha, int *coluna,VarStack *p) {
	int var_type;
	TvarDecl var;
	if (lookahead->value == INT || lookahead->value == FLOAT || lookahead->value == CHAR) {
		var_type = lookahead->value;
		*lookahead = Scan(arq, linha, coluna);
		if (lookahead->value == ID) {
			strcpy(var.lexema, lookahead->lexema);
			var.tipo = var_type;
			var.escopo = EscopoControl(0);
			if (VarInEscopo(p, lookahead->lexema, EscopoControl(0))==1) {
				printf("ERRO na linha %d, coluna %d, ultimo token lido t: Variavel ja declarada! \n",*linha,*coluna);
			}
			else {
				push(p, var);
			}	//verifica se existe && PUSH()
			*lookahead = Scan(arq, linha, coluna);
			while (lookahead->value == VIRGULA) {
				*lookahead = Scan(arq, linha, coluna);
				if (lookahead->value == ID) {
					strcpy(var.lexema, lookahead->lexema);
					var.tipo = var_type;
					var.escopo = EscopoControl(0);
					if (VarInEscopo(p, lookahead->lexema, EscopoControl(0)) == 1) {
						printf("ERRO na linha %d, coluna %d, ultimo token lido t: Variavel ja declarada! \n",*linha,*coluna);
					}
					else {
						push(p, var);
					}	//verifica se existe && PUSH()
					*lookahead = Scan(arq, linha, coluna);
					continue;
				}
				else {
					printf("ERRO na linha %d, coluna %d, ultimo token lido t: identificador nao encontrado \n", *linha, *coluna);//erro
				}
			}
			if (lookahead->value == PV) {
				*lookahead = Scan(arq, linha, coluna);
				return;
			}
			else {
				printf("ERRO na linha %d, coluna %d, ultimo token lido t: \";\" não encontrado \n", *linha, *coluna); //erro
			}
		}
	}
	else {
		printf("ERRO na linha %d, coluna %d, ultimo token lido t: tipo de declaração não encontrado \n", *linha, *coluna);//erro
	}
}
void CMD(FILE *arq, Ttoken *lookahead, int *linha, int *coluna, VarStack *p) {
	if (lookahead->value == ABREBLOCO || lookahead->value == ID) {
		CMDBASICO(arq, lookahead, linha, coluna,p);
		return;
	}
	else if (lookahead->value == WHILE || lookahead->value == DO) {
		//testar se dá pra fazer controle de escopo aqui
		ITERACAO(arq, lookahead, linha, coluna,p);
		return;
	}
	else if (lookahead->value == IF) {
		//testar se dá pra fazer controle de escopo aqui
		IFSTMT(arq, lookahead, linha, coluna,p);
		return;
	}
	else {
		printf("ERRO na linha %d, coluna %d, ultimo token lido t: comando esperado \n", *linha, *coluna);
		return;
	}
}

void BLOCO(FILE *arq, Ttoken *lookahead, int *linha, int *coluna,VarStack *p) {
	int escopo;
	if (lookahead->value == ABREBLOCO) {
		escopo = EscopoControl(1);
		*lookahead = Scan(arq, linha, coluna);
		while (lookahead->value == INT || lookahead->value == FLOAT || lookahead->value == CHAR) {
			DECLVAR(arq, lookahead, linha, coluna,p);
			//*lookahead = Scan(arq, linha, coluna);
		}
		while (lookahead->value == ABREBLOCO || lookahead->value == ID || lookahead->value == WHILE || lookahead->value == DO || lookahead->value == IF) {
			CMD(arq, lookahead, linha, coluna,p);
			//*lookahead = Scan(arq, linha, coluna);
		}
		if (lookahead->value == FECHABLOCO) {
			*lookahead = Scan(arq, linha, coluna);
			deleteEscopo(p, escopo);
			//deleta TODAS AS VARIAVEIS DO ESCOPO
			escopo = EscopoControl(-1);
			return;
		}
		else {
			printf("ERRO na linha %d, coluna %d, ultimo token lido t: fechamento de bloco esperado \n", *linha, *coluna);
		}
	}
	else {
		printf("ERRO na linha %d, coluna %d, ultimo token lido t: abertura de bloco esperada \n", *linha, *coluna);
	}

}
/*
void getProxToken(FILE *arq, Ttoken *t, int *linha, int *coluna) {
*t = Scan(arq,linha,coluna);
}*/

void PROGRAM(FILE *arq) {
	static int linha = 1;
	static int coluna = 1;
	Ttoken lookahead;
	VarStack p=NULL;

	criarStack(p);

	lookahead = Scan(arq, &linha, &coluna);
	if (lookahead.value == INT) {
		lookahead = Scan(arq, &linha, &coluna);
		if (lookahead.value == MAIN) {
			lookahead = Scan(arq, &linha, &coluna);
			if (lookahead.value == ABREPARENTESES) {
				lookahead = Scan(arq, &linha, &coluna);
				if (lookahead.value == FECHAPARENTESES) {
					lookahead = Scan(arq, &linha, &coluna);
					BLOCO(arq, &lookahead, &linha, &coluna,&p);
					//lookahead = Scan(arq, &linha, &coluna);
					if (lookahead.value == EOFF) {
						return; //sucesso
					}
					else {
						printf("ERRO na linha %d , coluna %d , ultimo token lido t: fim de arquivo esperado\n", linha, coluna);
					}
				}
				else {
					printf("ERRO na linha %d, coluna %d, ultimo token lido t: fechamento de parenteses esperado \n", linha, coluna);
				}
			}
			else {
				printf("ERRO na linha %d, coluna %d, ultimo token lido t: abertura de parenteses esperada  \n", linha, coluna);
			}
		}
		else {
			printf("ERRO na linha %d, coluna %d, ultimo token lido t: funcao \"main\" esperada \n", linha, coluna);
		}
	}
	else {
		printf("ERRO na linha %d, coluna %d, ultimo token lido t: Tipo de funcao \"int\" esperado \n", linha, coluna);
	}


}
