#pragma once
#include"grammaAnalysis.h"

//�������λ��
Error errorPos[100];
int errorPtr = 0;

int indexOfDynArr = 0;
//��¼��������������������ƥ��
int bracketNum = 0;
TkWord* ptrOfDynArr = NULL;
bool equality();

//�жϵ�ǰ�ַ����ǲ��ǻ����ַ�
bool primary() {
	TkWord* word = ptrOfDynArr;
	int tkcode = word->tkcode;

	//ÿ��primary������ȡһ���ַ���
	ptrOfDynArr = ptrOfDynArr->next;

	//��ǰ�ַ���ƴд������ߴ��ڷǷ��ַ�
	if (tkcode < -1) {
		errorPos[errorPtr].type = word->spelling;
		errorPos[errorPtr++].line = word->raw;
		return false;
	}
	//��ǰ�ַ��������ͳ������ַ��������߱�ʶ����������
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
	//��ǰ�ַ����ǲ�����
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
	//��ǰ�ַ������ڱ��ʽ
	if (word->spelling[0] == '(') {
		bracketNum++;
		return expression();
	}
	if (word->spelling[0] == ')') {
		//���Ų�ƥ��
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
	//��������������﷨
	else {
		if (word) {
			errorPos[errorPtr].type = word->spelling;
			errorPos[errorPtr++].line = word->raw;
		}
		return false;
	}
}

//�жϵ�ǰ�ַ����ǲ���һԪ���ʽ
bool unary() {
	//��ǰ�ַ���Ϊ'-'����'!'�����һ�������ַ����γ�һԪ���ʽ
	while(ptrOfDynArr->tkcode == 1 || ptrOfDynArr->spelling[0] == '!') {
		ptrOfDynArr = ptrOfDynArr->next;
	}
	return primary();
}

//�ж��ǲ��ǳ˳����ʽ
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
//�Ӽ�
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
//��ϵ���ʽ
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
//�еȱ��ʽ
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
//��ֵ���ʽ
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
		//�����Ÿ����ı��ʽ
		while ((ptrOfDynArr != NULL) && ptrOfDynArr->spelling[0] == ',') {
			ptrOfDynArr = ptrOfDynArr->next;
			ret = expression();
		}
		//���������б��ʽ
		if (ptrOfDynArr != NULL && (ptrOfDynArr->spelling[0] == ';' || ptrOfDynArr->spelling[0] == '}')) {
			indexOfDynArr++;
			continue;
		}
		//���ʽ���ԷֺŽ�����������˴���
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