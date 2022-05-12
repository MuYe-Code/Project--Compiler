#pragma once
#include"grammaAnalysis.h"

//保存错误位置
Error errorPos[100];
int errorPtr = 0;

int indexOfDynArr = 0;
//记录左括号数量，用于数量匹配
int bracketNum = 0;
TkWord* ptrOfDynArr = NULL;
bool equality();

//判断当前字符串是不是基本字符
bool primary() {
	TkWord* word = ptrOfDynArr;
	int tkcode = word->tkcode;

	//每次primary操作读取一个字符串
	ptrOfDynArr = ptrOfDynArr->next;

	//当前字符串拼写错误或者存在非法字符
	if (tkcode < -1) {
		errorPos[errorPtr].type = word->spelling;
		errorPos[errorPtr++].line = word->raw;
		return false;
	}
	//当前字符串是整型常量，字符常量或者标识符（变量）
	if (tkcode == TK_CINT || tkcode == TK_CCHAR || tkcode == TK_IDENT) {
		if (ptrOfDynArr != NULL && ptrOfDynArr->spelling[0] == ')') {
			if (bracketNum < 1) {
				errorPos[errorPtr].type = ptrOfDynArr->spelling;
				errorPos[errorPtr++].line = ptrOfDynArr->raw;
				return false;
			}
			else bracketNum--;
			ptrOfDynArr = ptrOfDynArr->next;
		}
		return true;
	}
	//当前字符串是布尔量
	if (!strcmp(word->spelling, "true") || !strcmp(word->spelling, "false")) {
		if (ptrOfDynArr != NULL && ptrOfDynArr->spelling[0] == ')') {
			if (bracketNum < 1) {
				errorPos[errorPtr].type = ptrOfDynArr->spelling;
				errorPos[errorPtr++].line = ptrOfDynArr->raw;
				return false;
			}
			else bracketNum--;
			ptrOfDynArr = ptrOfDynArr->next;
		}
		return true;
	}
	//当前字符串紧邻表达式
	if (word->spelling[0] == '(') {
		bracketNum++;
		return expression();
	}
	if (word->spelling[0] == ')') {
		//括号不匹配
		if (bracketNum < 1) {
			errorPos[errorPtr].type = word->spelling;
			errorPos[errorPtr++].line = word->raw;
			return false;
		}
		bracketNum--;
		return true;
	}
	/*if (word->spelling = ';') {
		indexOfDynArr++;
		ptrOfDynArr = NULL;
		return true;
	}*/
	//其余情况不符合语法
	else {
		if (word) {
			errorPos[errorPtr].type = word->spelling;
			errorPos[errorPtr++].line = word->raw;
		}
		return false;
	}
}

//判断当前字符串是不是一元表达式
bool unary() {
	//当前字符串为'-'或者'!'，与后一个基本字符串形成一元表达式
	while(ptrOfDynArr->tkcode == 1 || ptrOfDynArr->spelling[0] == '!') {
		ptrOfDynArr = ptrOfDynArr->next;
	}
	return primary();
}

//判断是不是乘除表达式
bool factor() {
	bool ret = unary();
	if (ret) {
		while (ptrOfDynArr->tkcode == 2 || ptrOfDynArr->tkcode == 3) {
			ptrOfDynArr = ptrOfDynArr->next;
			ret = factor();
		}
	}
	return ret;
}
//加减
bool term() {
	int ret = factor();
	if (ret) {
		while (ptrOfDynArr->tkcode == 0 || ptrOfDynArr->tkcode == 1) {
			ptrOfDynArr = ptrOfDynArr->next;
			factor();
		}
	}
	return ret;
}
//关系表达式
bool comparison() {
	int ret = term();
	if (ret) {

		while (ptrOfDynArr->tkcode >=7 && ptrOfDynArr->tkcode <= 10) {
			ptrOfDynArr = ptrOfDynArr->next;
			ret = term();
		}
	}
	return ret;
}
//判等表达式
bool equality() {
	int ret = comparison();
	if (ret) {
		while(ptrOfDynArr->tkcode == TK_EQ || ptrOfDynArr->tkcode == TK_NEQ) {
			ptrOfDynArr = ptrOfDynArr->next;
			ret = comparison();
		}
	}
	return ret;
}
//赋值表达式
bool expression() {
	int ret = equality();
	if (ret) {
		while(ptrOfDynArr->tkcode == TK_ASSIGN) {
			ptrOfDynArr = ptrOfDynArr->next;
			ret = equality();
		}
	}
	return ret;
}

void wordAnalysis(DynArray* arr, int n) {
	ptrOfDynArr = NULL;
	indexOfDynArr = 0;
	
	while (n > indexOfDynArr) {
		bracketNum = 0;
		ptrOfDynArr = arr->data[indexOfDynArr];
		while (ptrOfDynArr != NULL && ptrOfDynArr->spelling[0] == '\t') {
			ptrOfDynArr = ptrOfDynArr->next;
		}
		
		bool ret = expression();
		//处理逗号隔开的表达式
		while ((ptrOfDynArr != NULL) && ptrOfDynArr->spelling[0] == ',') {
			ptrOfDynArr = ptrOfDynArr->next;
			ret = expression();
		}
		//处理完所有表达式
		if (ptrOfDynArr != NULL && (ptrOfDynArr->spelling[0] == ';' || ptrOfDynArr->spelling[0] == '}')) {
			indexOfDynArr++;
			continue;
		}
		//表达式不以分号结束，则出现了错误
		if (ret && ptrOfDynArr != NULL) {
			errorPos[errorPtr].type = ptrOfDynArr->spelling;
			errorPos[errorPtr++].line = ptrOfDynArr->raw;
		}
		indexOfDynArr++;
	}
	ptrOfDynArr = NULL;
	indexOfDynArr = 0;
}

void printError() {
	puts("\n");
	printf("%d errors!\n", errorPtr);
	for (int i = 0; i < errorPtr; i++) {
		printf("line %-2d:\t%s\n", errorPos[i].line,errorPos[i].type);
	}
}