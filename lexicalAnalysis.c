#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "lexicalAnalysis.h"
#include <Windows.h>



void printColor(char* ch, TokenCode token) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	if (token >= TK_IDENT) SetConsoleTextAttribute(h, FOREGROUND_INTENSITY);//������ɫ 
	else if (token >= KW_CHAR) SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);// �ؼ�����ɫ 
	else if (token >= TK_CINT) SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN);//������ɫ 
	else SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);//���� ����ɫ

	if (-1 == ch[0]) { printf("\n End_Of_File!"); 
	SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); }
	else {printf("%s", ch); }
}//����tokenֵ��ȡ��Ӧ�ַ������ͣ�Ȼ���ӡ


//���ݹ�ϣ��ʽ�õ��ַ����Ĺ�ϣֵ
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

//��ʼ���ؼ��ֹ�ϣ��
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

//��ѯ�ַ�����TKֵ���������ڷ���100
TokenCode getTkcode(char* key) {
	int pos = elf_hash(key);
	TkWord* tmp = tk_hashtable[pos];
	while (tmp != NULL) {
		if (strcmp(key, tmp->spelling) == 0) return tmp->tkcode;
		else tmp = tmp->next;
	}
	return TK_IDENT;
}

//��һ�������붯̬���飬�Էֺ���Ϊ�־�ı�־��һ������һ��������
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

//��ȡ�ļ������ַ������뵥�ʱ�
void readFile(FILE* fp, DynArray* arr) {
	//char* oneLine[MAXKEY];
	char* wordBuf = NULL;
	char ch = '\0';
	int line = 1;
	ch = fgetc(fp);
	while (ch != EOF) {
		//ÿ��Ϊ���������ַ�������һ������,cur�Ǹ����������
		wordBuf = (char*)malloc(16);
		if (wordBuf == NULL) {
			perror("Error in readFile");
			exit(EXIT_FAILURE);
		}
		int cur = 0;
		//ȥ���հ��ַ�
		while (ch == ' ' || ch == '\t') ch = fgetc(fp);
		while (ch == '\n') {
			line++;
			ch = fgetc(fp);
		}

		switch (ch) {
			//�����������
			case '+': case '*': case '%': case ':':
			case '.': case '&': case '(': case ')':
			case '[': case ']': case '{': case '}':
			case ';': case ',':
				wordBuf[cur++] = ch;
				wordBuf[cur] = '\0';
				addToDynArr(wordBuf, arr, line,-1);
				ch = fgetc(fp);
				break;
			//����Ƿ񻹸��ŵȺ�
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
			//����Ƿ����'->'
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
			//�ַ�������
			case '"':
				//Ϊ�ַ�����������ϴ���ڴ�ռ�
				wordBuf = realloc(wordBuf, 40);
				if (wordBuf == NULL) {
					perror("Error in readFile");
					exit(EXIT_FAILURE);
				}
				do {
					wordBuf[cur++] = ch;
					ch = fgetc(fp);
				} while (ch != '"' && ch != '\n');
				//���ִ���
				if (ch != '"') {
					wordBuf[cur] = '\0';
					addToDynArr(wordBuf, arr, line, -2);
				}
				//�γ��ַ�������
				else {
					wordBuf[cur++] = ch;
					wordBuf[cur] = '\0';
					addToDynArr(wordBuf, arr, line,TK_CSTR);
					ch = fgetc(fp);
				}
				break;
			//�ַ�����
			case '\'':
				do {
					wordBuf[cur++] = ch;
					ch = fgetc(fp);
				} while (ch != '\'' && ch != '\n');
				//���ִ���
				if (ch != '\'') {
					wordBuf[cur] = '\0';
					addToDynArr(wordBuf, arr, line, -2);
				}
				//�γ��ַ�����
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
				//�γ�ע��
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
				//����һ���ַ������֣����ͳ���
				if (isdigit(ch)) {
					int flag = true;
					while (isdigit(ch)) {
						wordBuf[cur++] = ch;
						ch = fgetc(fp);
					}
					//���ֺ������ĸ���﷨����
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
				//����һ���ַ�����ĸ�����»��ߣ���ʶ�����߹ؼ���
				else if (isalpha(ch) || ch == '_') {
					while (isalnum(ch) || ch == '_') {
						wordBuf[cur++] = ch;
						ch = fgetc(fp);
					}
					wordBuf[cur] = '\0';
					addToDynArr(wordBuf, arr, line, -1);
					break;
				}
				//�Ƿ��ַ�
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

