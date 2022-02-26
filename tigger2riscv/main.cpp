#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "riscvgenerate.h"
#include "parser.tab.hpp"

extern FILE *yyin;
FILE *RISCV_code;


int main(int argc, char *argv[])
{
	char *infile = nullptr, *outfile = nullptr;
	int i = 1;
	for(;i < argc - 1;++i){
		if(strcmp(argv[i], "-S") == 0 && argv[i + 1][0] != '-' )
				infile = argv[i + 1];
		if(strcmp(argv[i], "-o") == 0 && argv[i + 1][0] != '-' )
				outfile = argv[i + 1];
	}

	if(infile == nullptr || outfile == nullptr)
		std::cerr << "Can not find or create the file." << std::endl;
	FILE *file;
	file = fopen(infile, "r");
	yyin = file;
	file = fopen(outfile,"w");
	RISCV_code = file;
	std::ofstream fout(outfile);
	std::streambuf *oldbuf = std::cout.rdbuf(fout.rdbuf());
	yyparse();
	PrintRiscV(SyntaxTree);
	std::cout.rdbuf(oldbuf);
	fout.close();
	return 0;



}

