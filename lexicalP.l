%{
#include<stdio.h> 	
#include <stdlib.h>
#include <string.h>
#include "syntP.tab.h"

int nb_ligne = 1, nb_colonne = 0;
int TabCpt=0 ;
int numLigne()
{
	return nb_ligne;
}
int numCol()
{
	return nb_colonne;
}

%}

idf [A-Z][a-z0-9]*
valE ((0|[1-9][0-9]*))|(\([-]([1-9][0-9]*)\))
com #+.*
opr  "=="|"<"|">"|"<="|">="|"!="
espace " "+
retour "\n"+

%%
[\t] {nb_colonne+=yyleng;TabCpt+=yyleng;  return tabulation;}
"=" {nb_colonne++; return aff;}
\, {nb_colonne++; return virgule;}
int {nb_colonne = nb_colonne + yyleng; return mc_entier;}
if {nb_colonne = nb_colonne + yyleng; return mc_if ;}
elif {nb_colonne = nb_colonne + yyleng; return mc_elif ;}
else {nb_colonne = nb_colonne + yyleng; return mc_else ;}
"FIN" {nb_colonne = nb_colonne + yyleng; return mc_fin ;}
":" {nb_colonne++; return dp;}
\( {nb_colonne++;return parouvr; }
\) { nb_colonne++;return parferm;}
"+" { nb_colonne++;return plus;}
"-" { nb_colonne++;return moins;}
"*" { nb_colonne++;return fois;}
"/" { nb_colonne++;return divis;}
{idf} {nb_colonne = nb_colonne + yyleng; yylval.chaine=strdup(yytext); return idf;}
{com} {nb_colonne = nb_colonne + yyleng; return comentaire;}
{opr} {nb_colonne=nb_colonne+strlen(yytext);yylval.chaine=strdup(yytext);return opr; }
{valE} {yylval.entier = atoi(yytext);

	
	 nb_colonne = nb_colonne + yyleng;
	 return valE;}
{espace} {nb_colonne = nb_colonne + yyleng; }
{retour} {nb_ligne+=yyleng; nb_colonne = 0; return retour ;}
. {printf("erreur lexicale ligne %d colonne %d\n", numLigne(), numCol());}


