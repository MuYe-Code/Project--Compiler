#define _CRT_SECURE_NO_WARNINGS
#include"lexicalAnalysis.h"
#include<stdlib.h>
#include"grammaAnalysis.h"
#include<windows.h>
#define SIZE(a) (sizeof(a)/sizeof(a[0]))

void printRst(DynArray* arr);

int main(void) {
	//存放关键字
	initHash(keywords, SIZE(keywords)-1);
	//存放读入的数组
	DynArray* arr = (DynArray*)calloc(1, sizeof(DynArray));
	arr->capacity = MAXKEY;
	
	FILE* fp = fopen("Cprogram.dat", "r");
	if (fp == NULL) {
		perror("Error in main");
		exit(EXIT_FAILURE);
	}
	readFile(fp, arr);
	fclose(fp);

	printRst(arr);
	//暂时只可以分析简单算术表达式
	wordAnalysis(arr,3);
	printError();
	destroyDynArr(arr);
	return 0;
}

//打印结果
void printRst(DynArray* arr) {
	int line = 1;
	TkWord* cur = NULL;
	printf("%4d|  ", line);
	for (int i = 0; i <= arr->count; i++) {
		cur = arr->data[i];
		while (cur != NULL) {
			int tmp = cur->tkcode;
			while (cur->raw > line) {
				line++;
				puts("");
				HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
				printf("%4d|  ", line);
			}
			printColor(cur->spelling, tmp);
			if (tmp > 24) putchar(' ');
			cur = cur->next;
		}
	}
}