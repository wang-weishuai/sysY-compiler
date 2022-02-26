#!/bin/zsh
lex -o lex.yy.cpp lex.l && yacc -d -o parser.tab.cpp parser.y -Wno-yacc && g++ -std=c++17 lex.yy.cpp parser.tab.cpp util.cpp main.cpp  -ll -ly -o compiler -Wno-deprecated-register