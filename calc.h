#ifndef CALC_H
#define CALC_H

#include <stdio.h>
#include <stdlib.h>

#define DIM_MAX 100


typedef enum { typeCon, typeId, typeOpr, typePunt } nodeEnum; 

typedef struct {
	char * name;
	int value;
	int attivo;
} entryVar;


typedef struct {
	char * name;
	entryVar * p;
	int attivo;
} entryPointer;

/* constants */
typedef struct {
    int value;                  /* value of constant */
} conNodeType;

/* identifiers */
typedef struct {
    entryVar * pos;                /* subscript to sym array */
} idNodeType;

/* pointers */
typedef struct {
	entryPointer * pos;
} puntNodeType;

/* operators */
typedef struct {
    int oper;                   /* operator */
    int nops;                   /* number of operands */
    struct nodeTypeTag **op;		/* operands */
} oprNodeType;


typedef struct nodeTypeTag {
    nodeEnum type;              /* type of node */
    union {
        conNodeType con;        /* constants */
        idNodeType id;          /* identifiers */
        oprNodeType opr;        /* operators */
        puntNodeType pt;			/* pointers */
    };
} nodeType;



/* prototypes */
entryVar * insertVariable(char * n);
entryVar * searchVariable(char * n);

entryPointer * insertPointer(char * n);
entryPointer * searchPointer(char * n);


nodeType * opr(int oper, int nops, ...);
nodeType * id(char * nome);
nodeType * con(int value);
nodeType * pt(char * nome);

void freeNode(nodeType *p);
int ex(nodeType *p);
         
void yyerror(char *s);       

#endif    

