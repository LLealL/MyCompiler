#ifndef TYPES
#define TYPES
#define tambuffer 512

typedef struct token {
	int value;
	char lexema[tambuffer];
}Ttoken;

typedef struct semantic {
	int type;
	char varTemp[16];
	char op;
}Tsemantic;

typedef struct opRelacional {
	int codOp;
	char string[2];

}TopRel;

typedef struct Wlabel {
	char begin[32];
	char end[32];
}Whilelabel;

typedef struct iflabel {
	char notIF[32];
	char end[32];
}IFlabel;

//identificador
#define ID -1
//PR
#define MAIN 1
#define IF 2
#define ELSE 3
#define WHILE 4
#define DO 5
#define FOR 6
#define INT 7
#define FLOAT 8
#define CHAR 9
#define VALCHAR 10
#define TYPEVOID 11

//SPEC
#define ABREBLOCO 13
#define FECHABLOCO 14
#define ABREPARENTESES 15
#define FECHAPARENTESES 16
#define VIRGULA 18
#define PV 19
#define ADD 20
#define SUB 21
#define MUL 22
#define DIV 23
#define ATRIB 24
#define MENORQ 25
#define MAIORQ 26
#define MENORIGUAL 27
#define MAIORIGUAL 28
#define IGUALIGUAL 29
#define DIFERENTE 30
#define INTEIRO 33
#define DECIMAL 34
#define EOFF 35
#define ERROR 36

#endif // ! TYPES
