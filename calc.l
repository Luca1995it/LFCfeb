/** calculator calc.l
 * origin source by Tom Niemann at epaperpress.com/lexandyacc
 * revision Lorenzo Massimo Gramola (2014)
 * revision Lorenzo Massimo Gramola (2015)
 * revision Lorenzo Massimo Gramola (2016) 
 */
%{
    #include <stdlib.h>
    #include "calc.h"
    #include "y.tab.h"
	

    void yyerror(char *);
%}

%%

[^"while"][^"print"][^"if"][^"else"][a-zA-Z][0-9A-Za-z]*          {
                    yylval.sIndex = strdup(yytext);
		    
                    return VARIABLE;
	            
                }
0               {
                    yylval.iValue = atoi(yytext);
                    return INTEGER;
                }
[1-9][0-9]*     {
                    yylval.iValue = atoi(yytext);
                    return INTEGER;
                }

[<][a-z][>]		{
                    yylval.sIndex = strdup(yytext);
                    return PUNTATORE;
                }

[-+()<>=*/;{}.]   { return *yytext; }

">="            return GE;
"<="            return LE;
"=="            return EQ;
"!="            return NE;
"while"         return WHILE;
"if"            return IF;
"else"          return ELSE;
"print"         return PRINT;


[ \t\n]         ;/* skip whitespace */

.               yyerror("Unknown character");

%%

int yywrap(void) {
    return 1;
}
