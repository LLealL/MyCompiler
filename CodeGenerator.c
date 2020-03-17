#include "CodeGenerator.h"
#include "string.h"


Tsemantic newVarTemp() {
	static int cont = 0;
	Tsemantic VarTemp;
	char string[15];
	char string2[15];

	VarTemp.type = TYPEVOID;
	VarTemp.op = '\0';
	VarTemp.varTemp[0] = '\0';


	string2[0] = 't';
	string2[1] = '\0';
	VarTemp.type = TYPEVOID;
	sprintf(string, "%d", cont);
	strcpy(VarTemp.varTemp,strcat(string2,string));

	cont = cont + 1;
	return VarTemp;
}

Whilelabel newWhileLabel() {
	static int cont = 0;
	Whilelabel newLabel;
	char string1[32];
	char string2[32];
	char string3[16];

	strcpy(string1, "WBegin");
	strcpy(string2, "WEnd");
	sprintf(string3, "%d", cont);
	strcpy(newLabel.begin, strcat(string1, string3));
	strcpy(newLabel.end, strcat(string2, string3));

	cont = cont + 1;

	return newLabel;
}
IFlabel newIFlabel() {
	static int cont = 0;
	IFlabel newLabel;
	char string1[32];
	char string2[32];
	char string3[16];

	strcpy(string1, "NotIF");
	strcpy(string2, "IfEnd");
	sprintf(string3, "%d", cont);
	strcpy(newLabel.notIF, strcat(string1, string3));
	strcpy(newLabel.end, strcat(string2, string3));

	cont = cont + 1;
	
	return newLabel;

}
