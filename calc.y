/** calculator calc.y
 * origin source by Tom Niemann at epaperpress.com/lexandyacc
 * revision Lorenzo Massimo Gramola (2014)
 * revision Lorenzo Massimo Gramola (2015): added node identifier for graph building
 * revision Lorenzo Massimo Gramola (2016): removed previusly added useless comments
*/

%{
    #include "calc.h"

    int yylex(void);

    void yyerror(char *s);       

%}

%union {
    int iValue;
    char* sIndex;
    nodeType *nPtr;
};                              /* this cause a new type def to be generated,
                                 which is a union of the above and and is called
                                 YYSTYPE. Then there is the declaration
                                 extern YYSTYPE yylval which declares yylval as
                                 an external variable
                                 */

%token <iValue> INTEGER         /* NOTE THIS DELCARATION*/
%token <sIndex> VARIABLE
%token WHILE IF PRINT
%nonassoc IFX
%nonassoc ELSE

%left GE LE EQ NE '>' '<'
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
        | '<' VARIABILE '>' ';'				{$$ = opr('d',1,pt($2));}
        | expr ';'                           {$$ = $1;}
        | PRINT expr ';'                     {$$ = opr(PRINT,1,$2);}
        | VARIABLE '=' expr ';'              {$$ = opr('=',2,id($1),$3);} 
        | WHILE '(' expr ')' stmt            {$$ = opr(WHILE,2,$3,$5);}
        | IF '(' expr ')' stmt %prec IFX     {$$ = opr(IF,2,$3,$5);}
        | IF '(' expr ')' stmt ELSE stmt     {$$ = opr(IF,3,$3,$5,$7);}
        | '{' stmt_list '}'                  {$$ = $2;}
		| VARIABILE '=' '@' VARIABLE ';'     {$$ = opr('p',2,pt($1),id($4));}
        ;


stmt_list:
        stmt                    { $$ = $1; }
        | stmt_list stmt        { $$ = opr(';', 2, $1, $2); }
        ;
expr:
        INTEGER                 {$$ = con($1);} //manage constants
        | VARIABLE              {$$ = id($1);} //manage variables - namely an IDENTIFIER
        | '<' VARIABILE '>'		{$$ = pt($2);}		
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
        | '(' expr ')'          {$$ = $2;
        ;

%%


void yyerror(char *s){
    fprintf(stdout,"%s\n",s);
}
int main(){
    yyparse();
    return 0;
}
