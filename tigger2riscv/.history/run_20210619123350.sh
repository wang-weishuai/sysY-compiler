#!/bin/zsh
lex -o lex.yy.cpp lex.l && yacc -d -o parser.tab.cpp parser.y -Wno-yacc && g++ -std=c++17 lex.yy.cpp parser.tab.cpp global.cpp eebasic.cpp eesymtab.cpp main.cpp eegenerate.cpp eeprint.cpp -ll -ly -o compiler -Wno-deprecated-register
if [[ $? -ne 0 ]]; then
	echo "\033[1;31msomething went wrong\033[0m"
fi
