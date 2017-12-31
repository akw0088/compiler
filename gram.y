%{
#include "include.h"
#define YYERROR_VERBOSE
#define YYSTYPE char *

/* Globals are bad */
SymbolTable	symbol_table;
CodeGen		codegen;
int ifcount = 0;


int yylex (void);
int yyerror(char *err);
%}

%token INT_LITERAL STRING_LITERAL ID INT CHAR BOOLEAN VOID T_BEGIN T_END IF ELSE CONTINUE NOT AND OR DO
%token GEQUAL LEQUAL EEQUAL NEQUAL EXP RETURNS RETURN
%left '-' '+'
%left '*' '/'
%right EXP

%%

program : decllist funclist
     | funclist
;

funclist : func funclist
     | func
;

func : RETURNS returntype ID { codegen.startfunc($3); } '(' parmlist ')' {symbol_table.push();} paramlist sblock {symbol_table.pop(); codegen.endfunc();}
;

parmlist : ID ',' parmlist
     | ID
     | 
;

decllist : decl ';' decllist
     | decl						{	codegen.endallocate();		}
     | 
;

decl : type ID sizelist					{	codegen.allocate($1, $2, $3);	}
     | type ID						{	codegen.allocate($1, $2, NULL);	}
;

paramlist : param ';' paramlist
     | param						{	codegen.endallocate_param();		}
     | 
;

param : type ID sizelist				{	codegen.allocate_param($1, $2, $3);	}
     | type ID						{	codegen.allocate_param($1, $2, NULL);	}
;

sizelist : size sizelist
{
	char *buffer = new char[strlen($1) + strlen($2) + 2];
	sprintf(buffer, "%s %s", $1, $2);
	$$ = buffer;
}
     | size
;

size : '[' INT_LITERAL ']'					{ $$ = $2; }
;

type : INT
     | CHAR
     | BOOLEAN
     | VOID
;

returntype : INT
     | CHAR
     | BOOLEAN
     | VOID
;

sblock : T_BEGIN { symbol_table.push(); } decllist stmts T_END { symbol_table.pop(); }
;

stmts :	stmt stmts
      | stmt
;


stmt : id '=' e ';'							{ 	codegen.assignment($1, $3);	}
     | ID '(' exprlist ')' ';' 					{	codegen.funccall($1);	}
     | id '=' conditionalexpr ';'
     | RETURN e ';'							{	codegen.ret($2);	}
     | IF '(' conditionalexpr ')' sblock
{
	codegen.ifblock((int)$3);
}
     | IF '(' conditionalexpr ')' sblock ELSE
{
	codegen.elseblock((int)$3);
}
 sblock
{
	codegen.endblock((int)$3);
}

     | DO INT_LITERAL '(' ID '=' dolist { codegen.midloop($2, $4);} stmts enddo {codegen.endloop($2, $6);}
;

enddo : T_END INT_LITERAL CONTINUE
;


conditionalexpr :
	  e relop e						{	$$ = (char *)++ifcount; codegen.conditional($1,$2,$3, ifcount, false);	}
	| NOT '(' conditionalexpr ')'				{	$$ = $3;	}
	| conditionalexpr AND '(' conditionalexpr ')'
	| conditionalexpr AND e relop e				{	codegen.conditional($3,$4,$5, ifcount, false);	}
	| conditionalexpr OR '(' conditionalexpr ')'
	| conditionalexpr OR e relop e				{	codegen.conditional($3,$4,$5, ifcount, false);	}
;


dolist :  e ';' e ')' T_BEGIN						{	codegen.startloop($1,"1"); $$ = $3;	}
     |  e ';' e ';' e ')' T_BEGIN						{	codegen.startloop($1,$5); $$ = $3;	}
;

exprlist : e ',' exprlist					{	codegen.push($1); }
     | e							{	codegen.push($1); }
     | {$$ = "";}
;

e : t
     | e '+' t							{	codegen.addsub("add", $1, $3); $$ = "stack";		}
     | e '-' t							{	codegen.addsub("sub", $1, $3); $$ = "stack";		}
;

t : p
     | t '*' p							{	codegen.muldiv("imul", $1, $3); $$ = "stack"; 	}
     | t '/' p							{	codegen.muldiv("idiv", $1, $3); $$ = "stack";		}
     | t '%' p							{	codegen.muldiv("mod", $1,$3); $$ = "stack";		}
;

p : f
     | p EXP INT_LITERAL					{	codegen.exponential($1, $3); $$ = "stack";	}
;

f : id
     | INT_LITERAL
     | STRING_LITERAL						{	$$ = codegen.allocatestr($1);		}
     | ID '(' exprlist ')'					{	codegen.funccall($1); $$ = "stack";	}
     | '(' e ')'						{ 	$$ = $2; 				}
;

id : ID
     | id '[' e ']'						{	codegen.handle_array($1, $3);	}
;

relop : '>'
     | '<'
     | GEQUAL
     | LEQUAL
     | EEQUAL
     | NEQUAL
;

%%

int yyerror(char *err)
{
        fflush(stdout);
	fprintf(stderr, "Syntax Error: %s\n", err);
        fflush(stdout);
	return 0;
}

int main()
{
	int rval = yyparse();

	if (rval != 0)
	{
		fprintf(stderr, "Language Rejected\n");
	}

	codegen.data();
	return rval;
}
