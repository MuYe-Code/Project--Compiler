#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "lexicalAnalysis.h"
#include <Windows.h>



void printColor(char* ch, TokenCode token) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	if (token >= TK_IDENT) SetConsoleTextAttribute(h, FOREGROUND_INTENSITY);//函数灰色 
	else if (token >= KW_CHAR) SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);// 关键字绿色 
	else if (token >= TK_CINT) SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN);//常量褐色 
	else SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);//运算 符红色

	if (-1 == ch[0]) { printf("\n End_Of_File!"); 
	SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); }
	else {printf("%s", ch); }
}//根据token值获取对应字符串类型，然后打印


//根据哈希公式得到字符串的哈希值
int elf_hash(char* key) {
	int h = 0, g;
	while (*key) {
		h = (h << 4) + *key++;
		g = h & 0xf0000000;
		if (g)
			h ^= g >> 24;
		h &= ~g;
		return h % MAXKEY;
	}
}

//初始化关键字哈希表
void initHash(TkWord keywords[], int len) {
	for (int i = 0; i < len; i++) {
		int pos = elf_hash(keywords[i].spelling);
		if (tk_hashtable[pos] == NULL) tk_hashtable[pos] = &keywords[i];
		else {
			TkWord* tmp = tk_hashtable[pos];
			while (tmp->next != NULL) tmp = tmp->next;
			tmp->next = &keywords[i];
		}
	}
}

//查询字符串的TK值，若不存在返回100
TokenCode getTkcode(char* key) {
	int pos = elf_hash(key);
	TkWord* tmp = tk_hashtable[pos];
	while (tmp != NULL) {
		if (strcmp(key, tmp->spelling) == 0) return tmp->tkcode;
		else tmp = tmp->next;
	}
	return TK_IDENT;
}

//将一个结点加入动态数组，以分号作为分句的标志，一句存放在一个链表中
void addToDynArr(char* str, DynArray* arr, int line, int tkcode) {
	TkWord* tmp = (TkWord*)malloc(sizeof(TkWord));
	if (tmp == NULL) {
		perror("Error in addToDynArr");
		exit(EXIT_FAILURE);
	}
	if (arr->cur == NULL) arr->data[arr->count] = tmp;
	else arr->cur->next = tmp;
	
	tmp->spelling = str;
	tmp->raw = line;
	tmp->tkcode = tkcode == -1 ? getTkcode(str) : tkcode;
	tmp->next = NULL;
	if (str[0] == ';'||str[0]=='}') {
		arr->count++;
		arr->data[arr->count] = NULL;
		arr->cur = NULL;
	}
	else arr->cur = tmp;
}

//读取文件，将字符串存入单词表
void readFile(FILE* fp, DynArray* arr) {
	//char* oneLine[MAXKEY];
	char* wordBuf = NULL;
	char ch = '\0';
	int line = 1;
	ch = fgetc(fp);
	while (ch != EOF) {
		//每次为接下来的字符串申请一个数组,cur是该数组的索引
		wordBuf = (char*)malloc(16);
		if (wordBuf == NULL) {
			perror("Error in readFile");
			exit(EXIT_FAILURE);
		}
		int cur = 0;
		//去除空白字符
		while (ch == ' ' || ch == '\t') ch = fgetc(fp);
		while (ch == '\n') {
			line++;
			ch = fgetc(fp);
		}

		switch (ch) {
			//单独的运算符
			case '+': case '*': case '%': case ':':
			case '.': case '&': case '(': case ')':
			case '[': case ']': case '{': case '}':
			case ';': case ',':
				wordBuf[cur++] = ch;
				wordBuf[cur] = '\0';
				addToDynArr(wordBuf, arr, line,-1);
				ch = fgetc(fp);
				break;
			//检查是否还跟着等号
			case '=': case '!': case '<': case '>':
				wordBuf[cur++] = ch;
				ch = fgetc(fp);
				if (ch == '=') {
					wordBuf[cur++] = ch;
					ch = fgetc(fp);
				}
				wordBuf[cur] = '\0';
				addToDynArr(wordBuf, arr, line, -1);
				break;
			//检查是否组成'->'
			case '-':
				wordBuf[cur++] = ch;
				ch = fgetc(fp);
				if (ch == '>') {
					wordBuf[cur++] = ch;
					ch = fgetc(fp);
				}
				wordBuf[cur] = '\0';
				addToDynArr(wordBuf, arr, line, -1);
				break;
			//字符串常量
			case '"':
				//为字符串常量申请较大的内存空间
				wordBuf = realloc(wordBuf, 40);
				if (wordBuf == NULL) {
					perror("Error in readFile");
					exit(EXIT_FAILURE);
				}
				do {
					wordBuf[cur++] = ch;
					ch = fgetc(fp);
				} while (ch != '"' && ch != '\n');
				//出现错误
				if (ch != '"') {
					wordBuf[cur] = '\0';
					addToDynArr(wordBuf, arr, line, -2);
				}
				//形成字符串常量
				else {
					wordBuf[cur++] = ch;
					wordBuf[cur] = '\0';
					addToDynArr(wordBuf, arr, line,TK_CSTR);
					ch = fgetc(fp);
				}
				break;
			//字符常量
			case '\'':
				do {
					wordBuf[cur++] = ch;
					ch = fgetc(fp);
				} while (ch != '\'' && ch != '\n');
				//出现错误
				if (ch != '\'') {
					wordBuf[cur] = '\0';
					addToDynArr(wordBuf, arr, line, -2);
				}
				//形成字符常量
				else {
					wordBuf[cur++] = ch;
					wordBuf[cur] = '\0';
					addToDynArr(wordBuf, arr, line, TK_CCHAR);
					ch = fgetc(fp);
				}
				break;

			case '/': 
				wordBuf[cur++] = ch;
				ch = fgetc(fp);
				//形成注释
				if (ch == '/') {
					wordBuf = realloc(wordBuf, 40);
					if (wordBuf == NULL) {
						perror("Error in readFile");
						exit(EXIT_FAILURE);
					}
					do {
						wordBuf[cur++] = ch;
						ch = fgetc(fp);
					} while (ch!=EOF && ch != '\n');
					wordBuf[cur] = '\0';
					addToDynArr(wordBuf, arr, line, TK_CSTR);
				}
				else {
					wordBuf[cur] = '\0';
					addToDynArr(wordBuf, arr, line, -1);
				}
				break;

			default: {
				//若第一个字符是数字，整型常量
				if (isdigit(ch)) {
					int flag = true;
					while (isdigit(ch)) {
						wordBuf[cur++] = ch;
						ch = fgetc(fp);
					}
					//数字后面接字母是语法错误
					if (isalpha(ch)) {
						flag = false;
						while (isalnum(ch) || ch == '_') {
							wordBuf[cur++] = ch;
							ch = fgetc(fp);
						}
					}
					wordBuf[cur] = '\0';
					addToDynArr(wordBuf, arr, line, (flag ? TK_CINT : -2));
					break;
				}
				//若第一个字符是字母或者下划线，标识符或者关键字
				else if (isalpha(ch) || ch == '_') {
					while (isalnum(ch) || ch == '_') {
						wordBuf[cur++] = ch;
						ch = fgetc(fp);
					}
					wordBuf[cur] = '\0';
					addToDynArr(wordBuf, arr, line, -1);
					break;
				}
				//非法字符
				else {
					wordBuf[cur++] = ch;
					wordBuf[cur] = '\0';
					ch = fgetc(fp);
					addToDynArr(wordBuf, arr, line, -2);
					break;
				}
			}
		}

	}
}

void destroyDynArr(DynArray* arr) {
	int count = arr->count;
	for (int i = 0; i < count; i++) {
		
		while (arr->data[i] != NULL) {
			TkWord* tmp = arr->data[i];
			arr->data[i] = tmp->next;
			if (tmp->spelling != NULL) free(tmp->spelling);
			free(tmp);
		}
	}
	free(arr);
}

