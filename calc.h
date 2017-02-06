/** calculator calc.y
 * origin source by Tom Niemann at epaperpress.com/lexandyacc
 * revision Lorenzo Massimo Gramola (2014)
 * revision Lorenzo Massimo Gramola (2015): added node identifier for graph building
 * revision Lorenzo Massimo Gramola (2016) 
 */
typedef enum { typeCon, typeId, typeOpr, typePunt } nodeEnum; /* used in the struct nodeType
                                                     to define the type of node*/

/* constants */
typedef struct {
    int value;                  /* value of constant */
} conNodeType;

/* identifiers */
typedef struct {
    varId * pos;                /* subscript to sym array */
} idNodeType;

/* pointers */
typedef struct {
	varPt * pos;
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
} varId;

typedef struct {
	char * name;
	varId * pt;
} varPt;


void insertVariable(char * n, int value);
varId * searchVariabile(char * n);

void insertPointer(char * n, nodeType * p);
varPt * searchPointer(char * n);

extern varId symTableVariabili[1000];           
extern varPr symTablePuntatori[1000];        

