#ifndef CALC_H
#define CALC_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "y.tab.h"

#define NUM_VAR 100


typedef enum { typeCon, typeId, typeOpr, typePunt } nodeEnum; /* used in the struct nodeType
                                                     to define the type of node*/

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
    int uid;
    union {
        conNodeType con;        /* constants */
        idNodeType id;          /* identifiers */
        oprNodeType opr;        /* operators */
        puntNodeType pt;			/* pointers */
    };
} nodeType;

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



/* prototypes */
entryVar * insertVariable(char * n, int value);
entryVar * searchVariabile(char * n);

entryPointer * insertPointer(char * n);
entryPointer * searchPointer(char * n);


nodeType *opr(int oper, int nops, ...);
nodeType *id(char * nome);
nodeType *con(int value);
nodeType *pt(char * nome);

void freeNode(nodeType *p);
int ex(nodeType *p);
         
entryVar * varTable[NUM_VAR];   
entryPointer * puntTable[NUM_VAR]; 

int dimVar = 0;
int dimPt = 0;

#endif    

