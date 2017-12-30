Digit		[0-9]
Letter		[a-zA-Z]
IntLit		{Digit}+

%{
#include "y.tab.h"

/*
	Notice that yylval memory is never deleted, should technically delete every nonused $ value in gram.y
but that would mean having a reduce action that deletes memory for every grammar longer than one token.
*/
#define STATEMENT yylval = (int) new char [strlen(yytext) + 1]; strcpy((char *)yylval, yytext);
%}

%%
[ \t\n]+				{ /* delete */	   			}
{IntLit}				{ STATEMENT; return INT_LITERAL;	}
void					{ STATEMENT; return VOID;		}
int					{ STATEMENT; return INT;		}
char					{ STATEMENT; return CHAR;		}
boolean					{ STATEMENT; return BOOLEAN;		}
if					{ STATEMENT; return IF;			}
"{"					{ STATEMENT; return T_BEGIN;		}
"}"					{ STATEMENT; return T_END;		}
returns					{ STATEMENT; return RETURNS;		}
return					{ STATEMENT; return RETURN;		}
else					{ STATEMENT; return ELSE;		}
continue				{ STATEMENT; return CONTINUE;		}
"&&"					{ STATEMENT; return AND;		}
"||"					{ STATEMENT; return OR;			}
do					{ STATEMENT; return DO;			}
not					{ STATEMENT; return NOT;		}
"+"					{ STATEMENT; return '+';		}
"-"					{ STATEMENT; return '-';		}
"*"					{ STATEMENT; return '*';		}
"**"					{ STATEMENT; return EXP;		}
"/"					{ STATEMENT; return '/';		}
"%"					{ STATEMENT; return '%';		}
"("					{ STATEMENT; return '(';		}
")"					{ STATEMENT; return ')';		}
"["					{ STATEMENT; return '[';		}
"]"					{ STATEMENT; return ']';		}
","					{ STATEMENT; return ',';		}
"'"					{ STATEMENT; return ';';		}
"="					{ STATEMENT; return '=';		}
">"					{ STATEMENT; return '>';		}
"<"					{ STATEMENT; return '<';		}
">="					{ STATEMENT; return GEQUAL;		}
"<="					{ STATEMENT; return LEQUAL;		}
"=="					{ STATEMENT; return EEQUAL;		}
"!="					{ STATEMENT; return NEQUAL;		}
";"					{ STATEMENT; return ';';		}
\"[^\"\n]*\"				{ STATEMENT; return STRING_LITERAL;	}
{Letter}({Letter}|{Digit}|_)*		{ STATEMENT; return ID;     		}

%%
int yywrap()
{
	return 1;
}