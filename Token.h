#pragma once
#include<stdio.h>
#define MAXKEY 100

/* 单词存储结构定义 */
typedef struct TkWord {
	int tkcode; // 单词编码，也就是token值 
	int raw;	//行号
	struct TkWord* next; // 指向哈希冲突的其它单词
	const char* spelling; // 单词字符串 
	//int state;
} TkWord;

/*动态数组定义*/
typedef struct DynArray {
	int count; // 动态数组元素个数 
	int capacity; // 动态数组缓冲区长度度
	TkWord* cur; //指向当前链表的最后一个结点
	TkWord* data[MAXKEY]; // 指向数据指针数组 
}DynArray;


/* 单词编码 */ 
typedef enum e_TokenCode {
	/* 运算符及分隔符 */ 
	//0-24
	TK_PLUS, // + 加号 
	TK_MINUS, // - 减号 
	TK_STAR, // * 星号 
	TK_DIVIDE, // / 除号 
	TK_MOD, // % 求余运算符 
	TK_EQ, // == 等于号 
	TK_NEQ, // != 不等于号 
	TK_LT, // < 小于号 
	TK_LEQ, // <= 小于等于号 
	TK_GT, // > 大于号 
	TK_GEQ, // >= 大于等于号 
	TK_ASSIGN, // = 赋值运算符 
	TK_POINTSTO, // -> 指向结构体成员运算符 
	TK_DOT, // . 结构体成员运算符 
	TK_AND, // & 地址与运算符 
	TK_OPENPA, // ( 左圆括号
	TK_CLOSEPA, // ) 右圆括号 
	TK_OPENBR, // [ 左中括号 
	TK_CLOSEBR, // ] 右圆括号 
	TK_BEGIN, // { 左大括号 
	TK_END, // } 右大括号 
	TK_SEMICOLON, // ; 分号 
	TK_COMMA, // , 逗号 
	TK_ELLIPSIS, // ... 省略号 
	TK_EOF, // 文件结束符 
	
	//25-27
	/* 常量 */ 
	TK_CINT, // 整型常量 
	TK_CCHAR, // 字符常量 
	TK_CSTR, // 字符串常量 
	
	//28-42
	/* 关键字 */ 
	KW_CHAR, // char关键字 
	KW_SHORT, // short关键字 
	KW_INT, // int关键字 
	KW_VOID, // void关键字 
	KW_STRUCT, // struct关键字 
	KW_IF, // if关键字 
	KW_ELSE, // else关键字 
	KW_FOR, // for关键字 
	KW_CONTINUE, // continue关键字 
	KW_BREAK, // break关键字 
	KW_RETURN, // return关键字 
	KW_SIZEOF, // sizeof关键字 
	KW_ALIGN, // __align关键字 
	KW_CDECL, // __cdecl关键字 standard c call 
	KW_STDCALL, // __stdcall关键字 pascal c call 
	
	//43
	/* 标识符 */ 
	TK_IDENT //函数 
}TokenCode;

static TkWord keywords[] = { 
	{TK_PLUS,0,NULL,"+"}, 
	{TK_MINUS,0,NULL,"-"},
	{TK_STAR,0,NULL,"*"},
	{TK_DIVIDE,0,NULL,"/"},
	{TK_MOD,0,NULL,"%"},
	{TK_EQ,0,NULL,"=="},
	{TK_NEQ,0,NULL,"!="},
	{TK_LT,0,NULL,"<"},
	{TK_LEQ,0,NULL,"<="},
	{TK_GT,0,NULL,">"},
	{TK_GEQ,0,NULL,">="},
	{TK_ASSIGN,0,NULL,"="},
	{TK_POINTSTO,0,NULL,"->"},
	{TK_DOT,0,NULL,"."},
	{TK_AND,0,NULL,"&"},
	{TK_OPENPA,0,NULL,"("},
	{TK_CLOSEPA,0,NULL,")"},
	{TK_OPENBR,0,NULL,"["},
	{TK_CLOSEBR,0,NULL,"]"},
	{TK_BEGIN,0,NULL,"{"},
	{TK_END,0,NULL,"}"},
	{TK_SEMICOLON,0,NULL,";"},
	{TK_COMMA,0,NULL,","},
	{TK_ELLIPSIS,0,NULL,"..."},
	{TK_EOF,0,NULL,"End_Of_File"},
	{TK_CINT,0,NULL,"整型常量"},
	{TK_CCHAR,0,NULL,"字符常量"},
	{TK_CSTR,0,NULL,"字符串常量"},
	{KW_CHAR,0,NULL,"char"},
	{KW_SHORT,0,NULL,"short"},
	{KW_INT,0,NULL,"int"},
	{KW_VOID,0,NULL,"void"},
	{KW_STRUCT,0,NULL,"struct"},
	{KW_IF,0,NULL,"if"},
	{KW_ELSE,0,NULL,"else"},
	{KW_FOR,0,NULL,"for"},
	{KW_CONTINUE,0,NULL,"continue"},
	{KW_BREAK,0,NULL,"break"},
	{KW_RETURN,0,NULL,"return"},
	{KW_SIZEOF,0,NULL,"sizeof"},
	{KW_ALIGN,0,NULL,"__align"},
	{KW_CDECL,0,NULL,"__cdecl"},
	{KW_STDCALL,0,NULL,"__stdcall"},
	{0} 
};

