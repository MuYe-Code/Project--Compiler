#pragma once
#include"Token.h"
#include<stdbool.h>

TkWord* tk_hashtable[MAXKEY];	// ���ʹ�ϣ��

void initHash(TkWord keywords[], int len);
TokenCode getTkcode(char* key); //��ѯ�ַ�����TKֵ
void readFile(FILE* fp, DynArray* arr);	//��ȡ�ļ������ַ������붯̬����
void printColor(char* ch, TokenCode token);	//��ӡ�ַ�����ɫ
int elf_hash(char* key);	//���ݹ�ϣ��ʽ�õ��ַ����Ĺ�ϣֵ
void addToDynArr(char* str, DynArray* arr, int line, int tkcode); //���������ַ������붯̬����(
void destroyDynArr(DynArray* arr);

