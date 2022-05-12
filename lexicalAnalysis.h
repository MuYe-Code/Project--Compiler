#pragma once
#include"Token.h"
#include<stdbool.h>

TkWord* tk_hashtable[MAXKEY];	// 单词哈希表

void initHash(TkWord keywords[], int len);
TokenCode getTkcode(char* key); //查询字符串的TK值
void readFile(FILE* fp, DynArray* arr);	//读取文件，将字符串存入动态数组
void printColor(char* ch, TokenCode token);	//打印字符串颜色
int elf_hash(char* key);	//根据哈希公式得到字符串的哈希值
void addToDynArr(char* str, DynArray* arr, int line, int tkcode); //将读到的字符串加入动态数组(
void destroyDynArr(DynArray* arr);

