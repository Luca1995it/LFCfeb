#include "calc.h"
#include "y.tab.h"
#include <stdarg.h>
#include <string.h>


entryVar * varTable[DIM_MAX];   
entryPointer * puntTable[DIM_MAX]; 

int dimVar = 0;
int dimPt = 0;

nodeType * con(int value){
    nodeType *p;
    /* allocate node space in memory */
    if((p=malloc(sizeof(nodeType))) == NULL){
        yyerror("out of memory");
    }
    /* copy information */
    p->type = typeCon;
    p->con.value = value;

    return p;
}

nodeType * id (char * nome){
    nodeType *p;
    if((p=malloc(sizeof(nodeType)))==NULL){
        yyerror("out of memory");
    }
    p->type = typeId;
    
    entryVar * tmp = searchVariable(nome);
    if(tmp == NULL){
    		p->id.pos = insertVariable(nome);
    	} else p->id.pos = tmp;

    return p;
}

nodeType * pt(char * nome){
	nodeType *p;
    if((p=malloc(sizeof(nodeType)))==NULL){
        yyerror("out of memory");
    }
    p->type = typePunt;
    
    entryPointer * tmp = searchPointer(nome);
    if(tmp == NULL){
    		p->pt.pos = insertPointer(nome);
    	} else p->pt.pos = tmp;

    return p;
}

nodeType * opr(int oper, int nops, ...){
    va_list ap; /* (ap = argument pointer) va_list is used to declare a variable
                 which, from time to time, is referring to an argument*/
    nodeType *p;
    int i;

    if ((p = malloc(sizeof(nodeType))) == NULL){
        yyerror("out of memory");
    }
    if((p->opr.op = malloc(nops*sizeof(nodeType)))== NULL){
        yyerror("out of memory");
    }
    p->type = typeOpr;
    p->opr.oper = oper;
    p->opr.nops = nops;
    va_start(ap, nops);/* initialize the sequence, makes ap point to the first
                        anonymous argument. Must call it once before reading all
                        the parameters*/
    for(i = 0; i<nops;i++){
        p->opr.op[i]=va_arg(ap,nodeType*); /*every time va_arg is called it returns
                                            an argument and moves the pointer forward
                                            to the next argument. It uses a type name
                                            to decide what kind of type to return and
                                            how much to move forward the pointer.
                                            */
    }
    va_end(ap); /* MUST be called BEFORE THE FUNCTION TERMINATES. it provides
                 the necessary cleaning functions.*/


    return p;
}


entryVar * insertVariable(char * n){
	if(((varTable[dimVar] = malloc(sizeof(entryVar))) == NULL) || (dimVar == DIM_MAX)){
		yyerror("out of memory");
	}
	varTable[dimVar]->name = n;
	varTable[dimVar]->value = 0;
	varTable[dimVar]->attivo = 0;
	return varTable[dimVar++];
}

entryVar * searchVariable(char * n){
	for(int i = 0; i < dimVar; i++){
		if(strcmp(varTable[i]->name,n) == 0) return varTable[i];
	}
	return NULL;
}

entryPointer * insertPointer(char * n){
	if(((puntTable[dimPt] = malloc(sizeof(entryPointer)))) == NULL || (dimVar == DIM_MAX)){
		yyerror("out of memory");
	}
	puntTable[dimPt]->name = n;
	puntTable[dimPt]->p = NULL;
	puntTable[dimPt]->attivo = 0;
	return puntTable[dimPt++];
}

entryPointer * searchPointer(char * n){
	for(int i = 0; i < dimPt; i++){
		if(strcmp(puntTable[i]->name,n) == 0) return puntTable[i];
	}
	return NULL;
}


void freeNode(nodeType *p){
    int i;
    if(!p) return;
    if(p->type == typeOpr){
        for(i=0;i<p->opr.nops; i++){
            freeNode(p->opr.op[i]);
        }
        free(p->opr.op);
    }
    free(p);
}


int ex(nodeType *p) {
    if (!p) return 0;
    switch(p->type) {
        case typeCon:       return p->con.value;
        case typeId:        	if(!p->id.pos->attivo){
        							yyerror("Var not declared");
        							return 0;
        						} else return p->id.pos->value;
        	case typePunt:		if(!p->pt.pos->attivo){
        							yyerror("Pointer not declared");
        							return 0;
        						} else if(p->pt.pos->p == NULL){
        							yyerror("Pointer not assigned");
        							return 0;
        						} else return p->pt.pos->p->value;
        case typeOpr:
            switch(p->opr.oper) {
                case WHILE:
                    while(ex(p->opr.op[0]))
                    {
                        ex(p->opr.op[1]);
                    }
                    return 0;
                case IF:
                    if (ex(p->opr.op[0])){
                        
                        ex(p->opr.op[1]);
                    }
                    else if (p->opr.nops > 2)
                    {
                        ex(p->opr.op[2]);
                    }
                    return 0;
                case PRINT:     printf("%d\n", ex(p->opr.op[0])); 
                					return 0;
                case ';':       ex(p->opr.op[0]); 
                					return ex(p->opr.op[1]);
                case 'd':		p->opr.op[0]->pt.pos->attivo = 1;
                					p->opr.op[0]->pt.pos->p = NULL;
                					return 0; 
                case 'p'	:		p->opr.op[0]->pt.pos->p = p->opr.op[1]->id.pos;
                					return 0;
                case '=':       p->opr.op[0]->id.pos->attivo = 1;
                					return p->opr.op[0]->id.pos->value = ex(p->opr.op[1]);
                case UMINUS:    return -ex(p->opr.op[0]);
                case '+':       return ex(p->opr.op[0]) + ex(p->opr.op[1]);
                case '-':       return ex(p->opr.op[0]) - ex(p->opr.op[1]);
                case '*':       return ex(p->opr.op[0]) * ex(p->opr.op[1]);
                case '/':       return ex(p->opr.op[0]) / ex(p->opr.op[1]);
                case '<':       return ex(p->opr.op[0]) < ex(p->opr.op[1]);
                case '>':       return ex(p->opr.op[0]) > ex(p->opr.op[1]);
                case GE:        return ex(p->opr.op[0]) >= ex(p->opr.op[1]);
                case LE:        return ex(p->opr.op[0]) <= ex(p->opr.op[1]);
                case NE:        return ex(p->opr.op[0]) != ex(p->opr.op[1]);
                case EQ:        return ex(p->opr.op[0]) == ex(p->opr.op[1]);
	
            }
    }
    return 0;
}
