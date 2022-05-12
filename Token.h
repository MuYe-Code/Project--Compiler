#pragma once
#include<stdio.h>
#define MAXKEY 100

/* ���ʴ洢�ṹ���� */
typedef struct TkWord {
	int tkcode; // ���ʱ��룬Ҳ����tokenֵ 
	int raw;	//�к�
	struct TkWord* next; // ָ���ϣ��ͻ����������
	const char* spelling; // �����ַ��� 
	//int state;
} TkWord;

/*��̬���鶨��*/
typedef struct DynArray {
	int count; // ��̬����Ԫ�ظ��� 
	int capacity; // ��̬���黺�������ȶ�
	TkWord* cur; //ָ��ǰ��������һ�����
	TkWord* data[MAXKEY]; // ָ������ָ������ 
}DynArray;


/* ���ʱ��� */ 
typedef enum e_TokenCode {
	/* ��������ָ��� */ 
	//0-24
	TK_PLUS, // + �Ӻ� 
	TK_MINUS, // - ���� 
	TK_STAR, // * �Ǻ� 
	TK_DIVIDE, // / ���� 
	TK_MOD, // % ��������� 
	TK_EQ, // == ���ں� 
	TK_NEQ, // != �����ں� 
	TK_LT, // < С�ں� 
	TK_LEQ, // <= С�ڵ��ں� 
	TK_GT, // > ���ں� 
	TK_GEQ, // >= ���ڵ��ں� 
	TK_ASSIGN, // = ��ֵ����� 
	TK_POINTSTO, // -> ָ��ṹ���Ա����� 
	TK_DOT, // . �ṹ���Ա����� 
	TK_AND, // & ��ַ������� 
	TK_OPENPA, // ( ��Բ����
	TK_CLOSEPA, // ) ��Բ���� 
	TK_OPENBR, // [ �������� 
	TK_CLOSEBR, // ] ��Բ���� 
	TK_BEGIN, // { ������� 
	TK_END, // } �Ҵ����� 
	TK_SEMICOLON, // ; �ֺ� 
	TK_COMMA, // , ���� 
	TK_ELLIPSIS, // ... ʡ�Ժ� 
	TK_EOF, // �ļ������� 
	
	//25-27
	/* ���� */ 
	TK_CINT, // ���ͳ��� 
	TK_CCHAR, // �ַ����� 
	TK_CSTR, // �ַ������� 
	
	//28-42
	/* �ؼ��� */ 
	KW_CHAR, // char�ؼ��� 
	KW_SHORT, // short�ؼ��� 
	KW_INT, // int�ؼ��� 
	KW_VOID, // void�ؼ��� 
	KW_STRUCT, // struct�ؼ��� 
	KW_IF, // if�ؼ��� 
	KW_ELSE, // else�ؼ��� 
	KW_FOR, // for�ؼ��� 
	KW_CONTINUE, // continue�ؼ��� 
	KW_BREAK, // break�ؼ��� 
	KW_RETURN, // return�ؼ��� 
	KW_SIZEOF, // sizeof�ؼ��� 
	KW_ALIGN, // __align�ؼ��� 
	KW_CDECL, // __cdecl�ؼ��� standard c call 
	KW_STDCALL, // __stdcall�ؼ��� pascal c call 
	
	//43
	/* ��ʶ�� */ 
	TK_IDENT //���� 
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
	{TK_CINT,0,NULL,"���ͳ���"},
	{TK_CCHAR,0,NULL,"�ַ�����"},
	{TK_CSTR,0,NULL,"�ַ�������"},
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

