#include <iostream>
#include <fstream>
#include <unistd.h>
#include "tiggerbasic.hpp"
#include "parser.tab.hpp"
extern FILE * yyin;
extern FILE * yyout;

using namespace std;

inline void myassert_fail(int line, const char* fn){
    printf("error: assertion failed\n%s (line %d)\n",fn, line);
    exit(5+line%100);
}

#define assert(x) do {if(!x) myassert_fail(__LINE__, __FILE__);}  while(0);

void Compile(){
  BaseEeyoreAST* root;
  yyparse(&root);
  dynamic_cast<ProgramAST *>(root)->generator();
}

int main(int argc, char *argv[]) {
 using std::string;
	int opt;
	char *infile = nullptr, *outfile = nullptr;
	int i = 1;
	for(;i < argc - 1;++i){
		if(strcmp(argv[i], "-t") == 0 && argv[i + 1][0] != '-' ) 
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
	yyout = file;
	std::ofstream fout(outfile);
	std::streambuf *oldbuf = std::cout.rdbuf(fout.rdbuf());
	BaseEeyoreAST* root;
	int IsyyparseSuccess = yyparse(&root);
	if(IsyyparseSuccess == 1) std::cerr << "invalid input" << std::endl;
	if(IsyyparseSuccess == 2) std::cerr << "memory exhaustion" << std::endl;
  streambuf* coutBuf = cout.rdbuf();
  ofstream of(outfile);
  streambuf* fileBuf = of.rdbuf();
  cout.rdbuf(fileBuf);
  dynamic_cast<ProgramAST *>(root)->generator();

}

