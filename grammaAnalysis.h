#pragma once
#include"Token.h"
#include<stdbool.h>
typedef struct error_s {
	int line;
	char* type;
}Error;
bool primary();
bool unary();
bool factor();
bool term();
bool comparison();
bool equality();
bool expression();
void wordAnalysis(DynArray* arr, int n);
void printError();
