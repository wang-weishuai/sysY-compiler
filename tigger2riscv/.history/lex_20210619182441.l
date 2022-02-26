aabb  22:36:57
[图片]
aabb  22:37:10
尤其是晚上
兰  22:37:13
夏天的味道和冬天
兰  22:37:15
不一样
aabb  22:37:16
在湖边
兰  22:37:17
但是
兰  22:37:17
都
aabb  22:37:22
小风那么一吹
兰  22:37:22
好喜欢啊
aabb  22:37:28
是的呀
aabb  22:37:36
[图片]
aabb  22:43:33

aabb  22:43:53
[图片]
aabb  23:07:27
啧
aabb  23:07:29
[图片]
aabb  00:36:19
uLezHCj8v1fMyhNJRDyQ
aabb  00:36:25
这个东西
aabb  00:36:29
是一个
aabb  00:36:37
【千万千万不能给别人看的东西】
aabb  00:36:43
存你这里了
兰  00:40:05
啥啊
aabb  00:40:17
git的密钥
兰  00:41:24
。。。
兰  00:41:26
好的
兰  00:41:30
完全不知道你在说什么
aabb  00:41:37
确实
aabb  00:41:39
我也不知道
aabb  00:41:45
我觉得我提交代码要交一个点
兰  00:53:46
啥网速啊
兰  00:53:48
[图片]
aabb  00:53:51
bu
aabb  00:53:52
是
aabb  00:53:54
很麻烦
aabb  00:53:59
提交代码的过程
aabb  00:54:01
专门有一个
aabb  00:54:08
1.2M的文档
aabb  00:54:14
先用gitlab托管
aabb  00:54:18
然后找到自己的token
aabb  00:54:22
和密钥结合
aabb  00:54:24
生成链接
aabb  00:55:47
https://MaxXing:-5dzfbjLsYsstYACyT2T@gitlab.eduxiji.net/MaxXing/pku-minic-test.git
aabb  01:00:12
https://pkuwws:uLezHCj8v1fMyhNJRDyQ@gitlab.eduxiji.net/pkuwws/wws_compiler.git
aabb  01:00:14
我试一下这个
aabb  01:07:34
草
aabb  01:07:35

下载源文件
得分0.00   最后一次提交时间:2021-06-19 01:07:07
CE

failed to compile the repository
compilation command: g++ -O2 -lm -std=c++17 -Wno-register -o /root/minic-grader/exec/compiler /root/minic-grader/exec/repo/eebasic.cpp /root/minic-grader/exec/repo/eegenerate.cpp /root/minic-grader/exec/repo/eeprint.cpp /root/minic-grader/exec/repo/eesymtab.cpp /root/minic-grader/exec/repo/global.cpp /root/minic-grader/exec/repo/main.cpp /root/minic-grader/exec/repo/lex.yy.cpp /root/minic-grader/exec/repo/parser.tab.cpp -I/root/minic-grader/exec/repo
reason:
/root/minic-grader/exec/repo/eegenerate.cpp: In function ‘std::__cxx11::string sym2str(int)’:
/root/minic-grader/exec/repo/eegenerate.cpp:38:1: warning: control reaches end of non-void function [-Wreturn-type]
}
^
/root/minic-grader/exec/repo/main.cpp: In function ‘int main(int, char**)’:
/root/minic-grader/exec/repo/main.cpp:36:6: error: ‘strcmp’ was not declared in this scope
if(strcmp(argv[i], "-e") == 0 && argv[i + 1][0] != '-' ) //In fact, it's not accurate but it can work.
^~~~~~
/root/minic-grader/exec/repo/main.cpp:36:6: note: ‘strcmp’ is defined in header ‘’; did you forget to ‘#include ’?
/root/minic-grader/exec/repo/main.cpp:23:1:
+#include

/root/minic-grader/exec/repo/main.cpp:36:6:
if(strcmp(argv[i], "-e") == 0 && argv[i + 1][0] != '-' ) //In fact, it's not accurate but it can work.
^~~~~~
/root/minic-grader/exec/repo/main.cpp:38:6: error: ‘strcmp’ was not declared in this scope
if(strcmp(argv[i], "-o") == 0 && argv[i + 1][0] != '-' )
^~~~~~
/root/minic-grader/exec/repo/main.cpp:38:6: note: ‘strcmp’ is defined in header ‘’; did you forget to ‘#include ’?

排行榜 »     


兰  01:12:20
睡了睡了
兰  01:12:23
明天看
aabb  01:12:40
安安啦
aabb  01:12:42
嘤嘤嘤
aabb  01:12:45
[大表情]
aabb  03:00:23
.
兰  09:05:57
？
aabb  10:03:12
吗的
aabb  10:03:21
四点才睡着
兰  14:42:44
午睡醒了还是困
兰  14:42:45
离谱
aabb  14:48:59
rua
aabb  16:22:16
https://pkuwws:uLezHCj8v1fMyhNJRDyQ@gitlab.eduxiji.net/pkuwws/tigger2riscv.git
aabb  16:25:35
w544MTc4DCpHRYeSupj3
https://pkuwws:w544MTc4DCpHRYeSupj3@gitlab.eduxiji.net/pkuwws/tigger2riscv.git
aabb  18:22:39
%{
#include "riscvgenerate.h"
#include "parser.tab.hpp"
#include <stdio.h>
#include <string>
#include <string.h>
#include <cstring>
extern "C" int input();
extern FILE* RISCV_code;
%}
letter		[a-zA-Z_]
digit		[0-9]
int 		-?{digit}+
id			{letter}+{digit}*{letter}*{digit}*
newline		\n
whitespace	[  \t]+
var          v[0-9]+|"x0"|s[0-9]|"s10"|"s11"|t[0-6]|a[0-7]
label		"l"{int}

%%
{var}			{
					yylval = NewNode(Exp_K,Var_K);
					yylval->attr.Var_info.index = atoi(yytext + 1);
					yylval->attr.Var_info.kind = yytext[0];
					return VAR;
				}

{label}			{
					yylval = NewNode(Exp_K,Var_K);
					yylval->attr.Var_info.index = atoi(yytext + 1);
					yylval->attr.Var_info.kind = yytext[0];
					return LABEL;
				}

"f_"{id} 		{
					yylval = NewNode(Func_K, Reserve_Type);
					strcpy(yylval->attr.func_name ,yytext);
					return FUN;
				}

"end"			{return END;}
"call"			{return CALL;}
"if"			{return IF;}
"goto"			{return GOTO;}
"return"		{return RETURN;}
"store"			{return STORE;}
"load"			{return LOAD;}
"loadaddr"		{return LOADADDR;}
"malloc"		{return MALLOC;}
"=="			{return EQ;}
"!="			{return NEQ;}
"&&" 			{return AND;}
"||"			{return OR;}

{int}			{yylval = NewNode(Exp_K,Const_K);
				 yylval->attr.val = atoi(yytext);
				 return INT;}
{newline}		{;}
{whitespace}	{;}
"//"			{ 
					for(auto i : std::string(yytext_ptr)) { if (i == '\n') yylineno++; }
				}
[\+\-\*\/\=\!\%\>\<\(\)\[\]\{\}\:]	{return yytext[0];}

%%

int yywrap()
{
	return 1;
}