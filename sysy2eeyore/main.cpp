/*
	Created by WangWeishuai.
	This compiler-project was started at April and it is too long till now.
	There are so many horrrrrrrible Errors (I think it is from linking when two hpp files contains each other),
	so it may looks not very elegant because some functions or methods can't work well but after changing a way to 
	express it which I think it is exactly the same as the first one I write before,
	(or just change a place to put some funcs), it runs miraculously:)
	I don’t want to explore the reason, because too long has passed and I can't remenber them:)
	Fortunately, Nearly all cases are solved correctly.
	I AM TOO VEGETABLE:)
*/

#pragma warning(disable:4996)

#include "eebasic.hpp"
#include "global.hpp"
#include "eegenerate.hpp"
#include "parser.tab.hpp"
#include "eeprint.hpp"

#include <iostream>
#include <fstream>


extern FILE *yyin, *yyout;
std::list<PointerOfBaseAST> whilestk;
PointerOfBaseAST curfundef;


int main(int argc, char *argv[]){
	using std::string;
	int opt;
	char *infile = nullptr, *outfile = nullptr;
	int i = 1;
	for(;i < argc - 1;++i){
		if(strcmp(argv[i], "-e") == 0 && argv[i + 1][0] != '-' ) //In fact, it's not accurate but it can work.
				infile = argv[i + 1];
		if(strcmp(argv[i], "-o") == 0 && argv[i + 1][0] != '-' )
				outfile = argv[i + 1];
	}

	if(infile == nullptr || outfile == nullptr)
		std::cerr << "Can not find or create the file." << std::endl;
	FILE *file;
	file = fopen(infile, "r");
	yyin = file;
	Filepreread(infile);
	file = fopen(outfile,"w");
	yyout = file;
	std::ofstream fout(outfile);
	std::streambuf *oldbuf = std::cout.rdbuf(fout.rdbuf());
	PointerOfBaseAST root;
	int IsyyparseSuccess = yyparse(&root);
	if(IsyyparseSuccess == 1) std::cerr << "invalid input" << std::endl;
	if(IsyyparseSuccess == 2) std::cerr << "memory exhaustion" << std::endl;
	Treatmain(root);
	TraverseAST(root);
	for(auto &i: eelines)
		EEDump(i);
	std::cout.rdbuf(oldbuf);
	fout.close();
}



