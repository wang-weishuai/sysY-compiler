#include "globals.h"
#include "util.h"
#include "Tigger2RISCV.tab.hpp"

FILE *RISCV_code;
extern FILE *yyin;

int main(int argc, char *argv[])
{

	yyin = fopen(argv[2], "r");
	RISCV_code = fopen(argv[4], "w");
	yyparse();
	convert(SyntaxTree);
	fclose(RISCV_code);
	fclose(yyin);
	return 0;
}

