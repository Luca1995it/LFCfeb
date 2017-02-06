
%{
    #include <stdlib.h>
    #include "calc.h"
    #include "y.tab.h"
	

    void yyerror(char *);
%}

%%


0               {
                    yylval.iValue = atoi(yytext);
                    return INTEGER;
                }
[1-9][0-9]*     {
                    yylval.iValue = atoi(yytext);
                    return INTEGER;
                }

[-+()<>=*/;{}@.]   { return *yytext; }

">="            return GE;
"<="            return LE;
"=="            return EQ;
"!="            return NE;
"while"         return WHILE;
"if"            return IF;
"else"          return ELSE;
"print"         return PRINT;

[a-zA-Z][0-9A-Za-z]*          {
                    yylval.sIndex = strdup(yytext);
                    return VARIABLE;
                }


[ \t\n]         ;	/* skip whitespace */

.               yyerror("Unknown character");

%%

int yywrap(void) {
    return 1;
}
