%{
#include<stdio.h> 	
#include <stdlib.h>
#include <string.h>
#include "quadP.h"
#include "pile.h"
extern FILE* yyin;
extern int yyaccept;
extern int TabCpt ;
Pile *p=NULL ;
int taillePile=0 ;
int MAJ =0 ;
int sauv[1000];
int cptsauv=0;
int sauvfin[1000];
int cptsauvfin=0;
int sauvfineif[100];
int cptfinelif=0;
int tempor=1;
char temp[20];
char str[12];
extern yytext;

int qc=0;
int saveIf=0;
int saveElif=0;
int saveElse=0;
int saveFinIf=0;
void indentation (int typeAppel){
	if(TabCpt>taillePile){
		yyerror() ;
	}else
	{
		while(TabCpt<taillePile){
			MAJ=depiler(p) ;
            sprintf(str, "%d", qc);
			ajour_quad(MAJ,1,str) ;

		}
		if(typeAppel	==1){
			empiler(p,qc) ;

		}

	}
}

%}
%union
{
char* chaine;
int entier;
float reel;
struct s {char * val; int type;}s;
}
%token <chaine>idf <chaine>comentaire <chaine>mc_entier dp <chaine>mc_if <chaine>mc_elif <chaine>mc_else <entier>valE egal moins virgule 
%token plus divis fois aff inferieur superieur infegal supegal different parouvr parferm espace retour tabulation mc_fin
%token <chaine> opr
%left infegal supegal superieur inferieur egal different
%left plus moins
%left fois divis
%type <s> EXP  OPERANDE EXPRIO
%start S
%%
S: DEC INSTRU FIN{ YYACCEPT;}
;
DEC: TYPE list retour DEC
   |
;
list:LISTEIDF |LISTEVAL ;
TYPE: mc_entier
;
LISTEIDF: LISTEIDF virgule idf  
		| idf
;
LISTEVAL: LISTEVAL virgule idf aff valE {sprintf(temp,"%d",$5); quadr("=",$3," ",temp);}  
		| idf aff valE {sprintf(temp,"%d",$3); quadr("=",$1," ",temp);}
;
INSTRU : INST  INSTRU  
       | 
       ;

INST: AFFEC retour
	| IF
	
;		
ELSEIF : {sprintf(temp,"%d",qc+1);
	ajour_quad(sauv[cptsauv-1],1,temp);
	 cptsauv--; sauvfin[cptsauvfin]=qc;cptsauvfin++;quadr("BR"," "," "," ");} ELIF{sauvfineif[cptfinelif]=qc;cptfinelif++;quadr("BR"," "," "," ");}ELSE{
						sprintf(temp,"%d",qc);			
						ajour_quad(sauvfin[cptsauvfin-1],1,temp);
						cptsauvfin--;
						ajour_quad(sauvfineif[cptfinelif-1],1,temp);
						cptfinelif--;
						
				} 
		|{sprintf(temp,"%d",qc);
		ajour_quad(sauv[cptsauv-1],1,temp);
	 cptsauv--;
	}
;
AFFEC: idf aff EXP{quadr("=",$3.val," ",$1);}
;
EXP: EXP plus EXPRIO{
	sprintf(temp,"T%d",tempor);
	tempor++;
	quadr("+",$1.val,$3.val,temp);
	$$.val=malloc(sizeof(20));strcpy($$.val,temp);
}
	
	|EXP moins EXPRIO{
	sprintf(temp,"T%d",tempor);
	tempor++;
	quadr("-",$1.val,$3.val,temp);
	$$.val=malloc(sizeof(20));strcpy($$.val,temp);
	}
	|EXPRIO {
		$$.val=malloc(sizeof(20));strcpy($$.val,$1.val);
	}
;
EXPRIO: EXPRIO fois OPERANDE{
	sprintf(temp,"T%d",tempor);
	tempor++;
	quadr("*",$1.val,$3.val,temp);
	$$.val=malloc(sizeof(20));strcpy($$.val,temp);
}
	|EXPRIO divis OPERANDE{
	sprintf(temp,"T%d",tempor);
	tempor++;
	quadr("/",$1.val,$3.val,temp);
	$$.val=malloc(sizeof(20));strcpy($$.val,temp);
	}
	|OPERANDE {$$.val=malloc(sizeof(20));strcpy($$.val,$1.val);}
;	
COMP: EXP opr EXP{  sauv[cptsauv]=qc;
					cptsauv++;
                                if(strcmp($2,"<")==0){
                                quadr("BGE","",$1.val,$3.val);
                                
                                }
                                if(strcmp($2,"==")==0) {
                                  quadr("BNE","",$1.val,$3.val);
                                
                                }
                                if(strcmp($2,"!=")==0) {
                                  quadr("BE","",$1.val,$3.val);  
                             
                                }
                                if(strcmp($2,">")==0){
                                  quadr("BLE","",$1.val,$3.val); 
                                
                                }
                                if(strcmp($2,"<=")==0){
                                  quadr("BG","",$1.val,$3.val);
                                }
                                if(strcmp($2,">=")==0){
                                  quadr("BL","",$1.val,$3.val);
                                 
                                }
                              }

;	
OPERANDE: idf {$$.val=malloc(sizeof(20));sprintf($$.val,"%s",$1);}
		|valE {$$.val=malloc(sizeof(20));sprintf($$.val,"%d",$1);}
;	
		
IF: mc_if CONDITION  dp retour TAB {printf(" un if\n");} ELSEIF INST

;
ELIF: mc_elif CONDITION dp retour TAB INST{
	sprintf(temp,"%d",qc+1);
	ajour_quad(sauv[cptsauv-1],1,temp);
	cptsauv--;
}
;
TAB: tabulation TAB | tabulation
ELSE: mc_else dp retour tabulation INST
;
CONDITION: parouvr COMP parferm
;
FIN : mc_fin
;	
%%

main ()
{
	
yyin=fopen("test.txt","r");
if(yyin==NULL){ printf("erreur d'ouverture");}
yyparse();
afficher_qdr();
tableToListe();
 generationCodeOBJ(); 
printf("\n \n");


}
	yywrap()
	{
		
	}
	int yyerror(char*  msg )
	{
		printf("erreur syntaxique ligne %d colonne %d", numLigne(), numCol());
	}
