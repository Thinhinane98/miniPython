flex lexicalP.l
bison -d syntP.y
gcc lex.yy.c syntP.tab.c -lfl -ly -o executable