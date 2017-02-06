%{
    #include "calc.h"
    
    int yylex(void);
	FILE * yyin;
%}

%union {
    int iValue;
    char* sIndex;
    struct nodeTypeTag *nPtr;
};                              

%token <iValue> INTEGER         /* NOTE THIS DELCARATION*/
%token <sIndex> VARIABLE
%token WHILE IF PRINT
%nonassoc IFX
%nonassoc ELSE

%left GE LE EQ NE '>' '<'
%right ';'
%left '+' '-'
%left '*' '/'
%nonassoc UMINUS

%type <nPtr> stmt expr stmt_list

%%

program:
        function                   {ex(NULL); exit(0);}
        ;

function:
        function stmt              {ex($2);freeNode($2);}
        | /* NULL */
        ;

stmt:
        ';'                                  {$$ = opr(';', 2, NULL, NULL);}
        | '<' VARIABLE '>' ';'				 {$$ = opr('d',1,pt($2));}
        | expr ';'                           {$$ = $1;}
        | PRINT expr ';'                     {$$ = opr(PRINT,1,$2);}
        | VARIABLE '=' expr ';'              {$$ = opr('=',2,id($1),$3);} 
        | WHILE '(' expr ')' stmt            {$$ = opr(WHILE,2,$3,$5);}
        | IF '(' expr ')' stmt %prec IFX     {$$ = opr(IF,2,$3,$5);}
        | IF '(' expr ')' stmt ELSE stmt     {$$ = opr(IF,3,$3,$5,$7);}
        | '{' stmt_list '}'                  {$$ = $2;}
		| VARIABLE '=' '@' VARIABLE ';'      {$$ = opr('p',2,pt($1),id($4));}
        ;


stmt_list:
        stmt                    { $$ = $1; }
        | stmt_list stmt        { $$ = opr(';', 2, $1, $2); }
        ;
expr:
        INTEGER                 {$$ = con($1);} //manage constants
        | VARIABLE              {$$ = id($1);} //manage variables - namely an IDENTIFIER
        | '<' VARIABLE '>'		{$$ = pt($2); }		
        | '-' expr %prec UMINUS {$$ = opr(UMINUS,1,$2);}
        | expr '+' expr         {$$ = opr('+',2,$1,$3);}
        | expr '-' expr         {$$ = opr('-',2,$1,$3);}
        | expr '*' expr         {$$ = opr('*',2,$1,$3);}
        | expr '/' expr         {$$ = opr('/',2,$1,$3);}
        | expr '<' expr         {$$ = opr('<',2,$1,$3);}
        | expr '>' expr         {$$ = opr('>',2,$1,$3);}
        | expr GE expr          {$$ = opr(GE,2,$1,$3);}
        | expr LE expr          {$$ = opr(LE,2,$1,$3);}
        | expr NE expr          {$$ = opr(NE,2,$1,$3);}
        | expr EQ expr          {$$ = opr(EQ,2,$1,$3);}
        | '(' expr ')'          {$$ = $2;}
        ;

%%


void yyerror(char *s){
    fprintf(stdout,"%s\n",s);
}

int main(int argc, char ** argv){
	if(argc != 2){	/* checking arguments rightness */
		yyerror("Invalid argument: use ./interpreter program");
		exit(0);
	}
	
	/* opening the input file for reading*/
	FILE * fp = fopen(argv[1],"r");
	
	/* checking that the open was successfully */
	if(fp != NULL){
		yyin = fp;
		yyparse();
		fclose(fp);
	} else yyerror("error opening file");   
    
    return 0;
}
