typedef struct qdr{

    char oper[100]; 
    char op1[100];   
    char op2[100];   
    char res[100];  
    
  }qdr;
#define INSERTION 1
#define SUPPRESSION 2

  qdr  quad[1000];

extern int  qc;


int qc1=0;



typedef struct ElementQuad ElementQuad;
typedef ElementQuad* ListQuadruplets;

typedef struct ElementQ ElementQ;
typedef ElementQ* PileQuad;

struct ElementQ{
    ListQuadruplets quad;
    PileQuad suivant;
};

struct ElementQuad { 
    char* opr;           
    char* op1;           
    char* op2;           
    char* res;
    ListQuadruplets suivant;
};
void quadr(char opr[],char op1[],char op2[],char res[])
{

	strcpy(quad[qc].oper ,opr);
	strcpy(quad[qc].op1   ,   op1);
	strcpy(quad[qc].op2   ,   op2);
	strcpy(quad[qc].res     ,   res);
	
	
qc++;

}
void ajour_quad(int num_quad, int colon_quad, char val [])
{ 
if (colon_quad==0)    strcpy(quad[num_quad].oper ,  val);
else if (colon_quad==1)   strcpy(quad[num_quad].op1  ,  val);
         else if (colon_quad==2)    strcpy(quad[num_quad].op2  ,   val);
                   else if (colon_quad==3)    strcpy(quad[num_quad].res  ,   val);

}
void afficher_qdr()
{
printf("*********************Les Quadruplets***********************\n");

int i;

for(i=0;i<qc+1;i++)
		{
 printf("\n %d - ( %s  ,  %s  ,  %s  ,  %s )",i,quad[i].oper,quad[i].op1,quad[i].op2,quad[i].res); 
 printf("\n---------------------------------------------------\n");
}
}


ListQuadruplets depilerQuad(PileQuad* S);
void empilerQuad(PileQuad* S,ListQuadruplets quad);
ListQuadruplets quadSuiv(ListQuadruplets p, char* opr,char* op1,char* op2,char* res);
ListQuadruplets suppQuad( ListQuadruplets p);
void expressionsRedondantes( ListQuadruplets LQ);
void expressionsPropagation(ListQuadruplets LQ);
void simplifierCalculs(ListQuadruplets LQ);
void variablesPropagation(ListQuadruplets LQ);
int estExpressionArithmetique(ListQuadruplets p);
int estAffectation(ListQuadruplets p);
void modifierQuad(int pos, int action);
int positionQuad(ListQuadruplets p);
int estNombre(char* ch);
void remplacer(char* enc, char* nouv);
int quadVide(PileQuad S);
int estBranchement(ListQuadruplets p);


ListQuadruplets LQ = NULL;

void expressionsRedondantes( ListQuadruplets LQ){
    ListQuadruplets p,q,s,r;
    char* temp1;
    char* temp2;
    int isConstant;
    PileQuad SQ = NULL;
    
    while(LQ != NULL){    
        if(!estExpressionArithmetique(LQ)){        
            LQ = LQ->suivant;
            continue;
        }
        
        p = LQ;
        r = p->suivant;
        q = r;
        
        while(r!=NULL){
            isConstant = 1;
            while(p!=NULL&&q!=NULL&&!strcmp(p->opr,q->opr)&&estExpressionArithmetique(p)){
                if(!strcmp(p->op1,q->op1)&&!strcmp(p->op2,q->op2)){
                    temp1 = strdup(p->res);
                    temp2 = strdup(q->res);
                }else if(!strcmp(p->op1,q->op1)&&!strcmp(temp1,p->op2)&&!strcmp(temp2,q->op2)){
                    temp1 = strdup(p->res);
                    temp2 = strdup(q->res);
                }else if(!strcmp(p->op2,q->op2)&&!strcmp(temp1,p->op1)&&!strcmp(temp2,q->op1)){
                    temp1 = strdup(p->res);
                    temp2 = strdup(q->res);
                }else{
                    break;
                }
                empilerQuad(&SQ,q);
                p = p->suivant;
                q = q->suivant;
            }
            if(p!=NULL&&q!=NULL&&estAffectation(p)&&estAffectation(q)&&!isConstant){ //Expressions équivalentes
                q->op1 = strdup(p->res);
                while(!quadVide(SQ)){
                    s = depilerQuad(&SQ);
                    modifierQuad(positionQuad(s),SUPPRESSION);
                    s = suppQuad(s);
                }
                p = LQ;
                r = s;
                q = r;
            }else{
                while(!quadVide(SQ)){
                    depilerQuad(&SQ);
                }
                p = LQ;
                r = r->suivant;
                q = r;
            }
        }
        
        LQ = LQ->suivant;
    }
}

void expressionsPropagation(ListQuadruplets LQ){
    ListQuadruplets p,q,s,r,t;
    char* temp1;
    char* temp2;
    int isConstant;
    PileQuad SQ = NULL;
    
    q=LQ;
    
    while(LQ != NULL){
        if(!estExpressionArithmetique(LQ)){   
            q = LQ->suivant;
            LQ = LQ->suivant;
            continue;
        }
        
        r = LQ->suivant;
        isConstant = 1;
        
       
        
        
        if(r!=NULL&&estAffectation(r)&&isConstant){
            p = r->suivant;
            
            while(p!=NULL){
                if(estAffectation(p)){
                    if(!strcmp(p->res,r->res)){
                        p = p->suivant;
                    }else if(!strcmp(p->op1,r->res)){
                        s = p;
                        while(q!=r){
                            modifierQuad(positionQuad(s->suivant),INSERTION);
                            s = quadSuiv(s,q->opr,q->op1,q->op2,q->res);
                            q = q->suivant;
                        }
                            modifierQuad(positionQuad(s->suivant),INSERTION);
                            s = quadSuiv(s,q->opr,q->op1,q->op2,q->res);
                            s->res = strdup(p->res);
                            modifierQuad(positionQuad(p),SUPPRESSION);
                            p = suppQuad(p);
                            LQ = p;
                            break;
                        
                    }else{
                        p = p->suivant;
                    }p = p->suivant;
                }else{
                    p = p->suivant;
                }
            }
            
            LQ = r->suivant;
        }else{
            if(r!=NULL){
                LQ = r->suivant;
            }else{
                LQ = NULL;
            }
        }
        
    }
}

void simplifierCalculs(ListQuadruplets LQ){
    ListQuadruplets p;
    while(LQ!=NULL){
        if(!strcmp(LQ->opr,"*")){
            if(estNombre(LQ->op1)&&!estNombre(LQ->op2)){
                int n = atoi(LQ->op1);
                
                if(n>1&&n<=10){
                    modifierQuad(positionQuad(LQ->suivant),INSERTION);
                    p = quadSuiv(LQ,"+",LQ->op2,LQ->op2,LQ->res);
                    n--;
                    while(n>1){
                        modifierQuad(positionQuad(p->suivant),INSERTION);
                        p = quadSuiv(p,"+",LQ->op2,LQ->res,LQ->res);
                        n--;
                    }
                    
                    modifierQuad(positionQuad(LQ),SUPPRESSION);
                    LQ = suppQuad(LQ);
                    continue;
                }else if(n==1){
                    remplacer(LQ->res,LQ->op2);
                    modifierQuad(positionQuad(LQ),SUPPRESSION);
                    LQ = suppQuad(LQ);
                    continue;
                }
            }else if(estNombre(LQ->op2)&&!estNombre(LQ->op1)){
                int n = atoi(LQ->op2);
                
                if(n>1&&n<=10){
                    modifierQuad(positionQuad(LQ->suivant),INSERTION);
                    p = quadSuiv(LQ,"+",LQ->op1,LQ->op1,LQ->res);
                    n--;
                    while(n>1){
                        modifierQuad(positionQuad(p->suivant),INSERTION);
                        p = quadSuiv(p,"+",LQ->op1,LQ->res,LQ->res);
                        n--;
                    }
                    
                    modifierQuad(positionQuad(LQ),SUPPRESSION);
                    LQ = suppQuad(LQ);
                    continue;
                }else if(n==1){
                    remplacer(LQ->res,LQ->op1);
                    modifierQuad(positionQuad(LQ),SUPPRESSION);
                    LQ = suppQuad(LQ);
                    continue;
                }
            }else{
                int n1 = atoi(LQ->op1);
                int n2 = atoi(LQ->op2);
                
                if(n1==1){
                    remplacer(LQ->res,LQ->op2);
                    modifierQuad(positionQuad(LQ),SUPPRESSION);
                    LQ = suppQuad(LQ);
                    continue;
                }else if(n2==1){
                    remplacer(LQ->res,LQ->op1);
                    modifierQuad(positionQuad(LQ),SUPPRESSION);
                    LQ = suppQuad(LQ);
                    continue;
                }else if(n1>1&&n1<=10){
                    modifierQuad(positionQuad(LQ->suivant),INSERTION);
                    p = quadSuiv(LQ,"+",LQ->op2,LQ->op2,LQ->res);
                    n1--;
                    while(n1>1){
                        modifierQuad(positionQuad(p->suivant),INSERTION);
                        p = quadSuiv(p,"+",LQ->op2,LQ->res,LQ->res);
                        n1--;
                    }
                    
                    modifierQuad(positionQuad(LQ),SUPPRESSION);
                    LQ = suppQuad(LQ);
                    continue;
                }else if(n2>1&&n2<=10){
                    modifierQuad(positionQuad(LQ->suivant),INSERTION);
                    p = quadSuiv(LQ,"+",LQ->op2,LQ->op2,LQ->res);
                    n2--;
                    while(n2>1){
                        modifierQuad(positionQuad(p->suivant),INSERTION);
                        p = quadSuiv(p,"+",LQ->op2,LQ->res,LQ->res);
                        n2--;
                    }
                    
                    modifierQuad(positionQuad(LQ),SUPPRESSION);
                    LQ = suppQuad(LQ);
                    continue;
                    
                }
                
            }
            
            LQ = LQ->suivant;
            
        }else if(!strcmp(LQ->opr,"+")){
            if(estNombre(LQ->op1)){
                int n = atoi(LQ->op1);
                
                if(n==0){
                    remplacer(LQ->res,LQ->op2);
                    modifierQuad(positionQuad(LQ),SUPPRESSION);
                    LQ = suppQuad(LQ);
                    continue;
                }
            }
            if(estNombre(LQ->op2)){
                int n = atoi(LQ->op2);
                
                if(n==0){
                    remplacer(LQ->res,LQ->op1);
                    modifierQuad(positionQuad(LQ),SUPPRESSION);
                    LQ = suppQuad(LQ);
                    continue;
                }
            }
                LQ = LQ->suivant;
            
        }else if(!strcmp(LQ->opr,"-")&&strcmp(LQ->op2,"")){
            if(estNombre(LQ->op2)){
                int n = atoi(LQ->op2);
                
                if(n==0){
                    remplacer(LQ->res,LQ->op1);
                    modifierQuad(positionQuad(LQ),SUPPRESSION);
                    LQ = suppQuad(LQ);
                }else{
                    LQ = LQ->suivant;
                }
            }else{
                LQ = LQ->suivant;
            }
        }else if(!strcmp(LQ->opr,"/")){
            if(estNombre(LQ->op2)){
                int n = atoi(LQ->op2);
                
                if(n==1){
                    remplacer(LQ->res,LQ->op1);
                    modifierQuad(positionQuad(LQ),SUPPRESSION);
                    LQ = suppQuad(LQ);
                }else{
                    LQ = LQ->suivant;
                }
            }else{
                LQ = LQ->suivant;
            }
        }else{
            LQ = LQ->suivant;
        }
        
    }
}
void variablesPropagation(ListQuadruplets LQ){
    ListQuadruplets p;
    int mustPropage;
    
    while(LQ != NULL){    
        if(estAffectation(LQ)){
            p = LQ->suivant;
            mustPropage = 0;
            while(p!=NULL&&(!estAffectation(p)||strcmp(LQ->res,p->res))){
                if(!strcmp(LQ->res,p->op1)){
                    p->op1 = strdup(LQ->op1);
                    mustPropage = 1;
                }
                if(!strcmp(LQ->res,p->op2)){
                    p->op2 = strdup(LQ->op1);
                    mustPropage = 1;
                }
                
                p = p->suivant;
            }
            
            if(mustPropage){
                modifierQuad(positionQuad(LQ),SUPPRESSION);
                LQ = suppQuad(LQ);
            }else{
                LQ = LQ->suivant;
            }
        }else{
           LQ = LQ->suivant; 
        }
        
    }
}


void optimiser( ListQuadruplets LQ){
	int a=qc1;
    expressionsRedondantes(LQ);
    expressionsPropagation(LQ);
    simplifierCalculs(LQ);
    variablesPropagation(LQ);
    if(!LQ){ 
        printf("vide\n");
    }
}

void afficherQuadOptimises(){
    int i=0;
    ListQuadruplets q = LQ;
    printf("********************* Les Quadruplets optimisés***********************\n");

    while(q!=NULL){
    printf("%d - (%s,%s,%s,%s)\n",i++,q->opr,q->op1,q->op2,q->res);
        q = q->suivant;
  }
  
  printf("\n");
}


void quadlist(char* opr,char* op1,char* op2,char* res){
    ListQuadruplets nouvelElt = (ListQuadruplets) malloc( sizeof(ElementQuad));
    
    nouvelElt->opr = opr;
    nouvelElt->op1 = op1;
    nouvelElt->op2 = op2;
    nouvelElt->res = res;
    nouvelElt->suivant = NULL;
    
    if(LQ == NULL){

        LQ=nouvelElt;

    }else{
        ListQuadruplets q = LQ;
        while(q->suivant!=NULL){
            q=q->suivant;
        }
        q->suivant = nouvelElt;
    }
}


void convertirTab(){

  char* oper= NULL;
  char* op1= NULL;
  char* op2= NULL;
  char* res= NULL;


int i=0;

for (i = 0; i < qc; ++i)
{
      oper = quad[i].oper;
      op1 = quad[i].op1;
      op2 = quad[i].op2;
      res = quad[i].res;

      quadlist(oper,op1,op2,res);


} 
qc1=qc;


optimiser(LQ);

afficherQuadOptimises();




}


int estAffectation(ListQuadruplets p){
    return !strcmp(p->opr,"=");
}

int estBranchement(ListQuadruplets p){
    return !strcmp(p->opr,"BR")||!strcmp(p->opr,"BE")||!strcmp(p->opr,"BNE")||!strcmp(p->opr,"BG")||!strcmp(p->opr,"BGE")||!strcmp(p->opr,"BL")||!strcmp(p->opr,"BLE");
}


int estNombre(char* ch){
    int i=0;
    while(i<strlen(ch)){
        if(!isdigit(ch[i])){
            return 0;
        }
        i++;
    }
    return 1;
}


ListQuadruplets depilerQuad(PileQuad* S){
    if(!quadVide(*S)){
        PileQuad p = *S;
        ListQuadruplets quad = p->quad;
        *S = p->suivant;
        free(p);
        return quad;
    }

    return NULL;
}

void empilerQuad(PileQuad* S,ListQuadruplets quad){
    PileQuad nouvelElt = (PileQuad) malloc(sizeof(ElementQ));

    nouvelElt->quad = quad;
    nouvelElt->suivant = *S;
    *S=nouvelElt;
}

int quadVide(PileQuad S){
    return S == NULL;
}


ListQuadruplets removeQuad(char* opr, char* op1, char* op2){
    
    if(LQ!=NULL){
        if(!strcmp(LQ->opr,opr)&&!strcmp(LQ->op1,op1)&&!strcmp(LQ->op1,op2)){
            ListQuadruplets temp = LQ;
            LQ = LQ->suivant;
            free(temp);
        }else{
            ListQuadruplets p = LQ;
            ListQuadruplets q = LQ->suivant;
            
            while(q!=NULL){
                if(!strcmp(LQ->opr,opr)&&!strcmp(LQ->op1,op1)&&!strcmp(LQ->op1,op2)){
                    p->suivant = q->suivant;
                    free(q);
                    break;
                }else{
                    p = q;
                    q = q->suivant;
                }
            }
        }
    }
    
    return LQ;
}

ListQuadruplets suppQuad( ListQuadruplets p){
    if(p!=NULL){
        if(p == LQ){
            LQ = LQ->suivant;
            free(p);
            return LQ;
        }else{
            ListQuadruplets q = LQ;
            
            while(q!=NULL && q->suivant != p){
                q = q->suivant;
            }
            
            if(q!=NULL){
                q->suivant = p->suivant;
                free(p);
                return q->suivant;
            }
        }
    }
    
    return LQ;
}


ListQuadruplets quadSuiv(ListQuadruplets p, char* opr,char* op1,char* op2,char* res){
    if(p!=NULL){
        ListQuadruplets nouvelElt = (ListQuadruplets) malloc(sizeof(ElementQuad));

        nouvelElt->opr = opr;
        nouvelElt->op1 = op1;
        nouvelElt->op2 = op2;
        nouvelElt->res = res;
        nouvelElt->suivant = p->suivant;
        
        p->suivant = nouvelElt;
        
        return nouvelElt;
        
    }else{
        return NULL;
    }
}

int positionQuad(ListQuadruplets p){
    int i=1;
    ListQuadruplets q = LQ;
    
    while(q != NULL && q != p){
        i++;
        q = q->suivant;
    }
    
    if(q==NULL){
        return 0;
    }else{
        return i;
    }
}

void remplacer(char* oldTemp, char* newTemp){
    ListQuadruplets p = LQ;
    
    while(p!=NULL){
        if(!strcmp(p->op1,oldTemp)){
            p->op1 = strdup(newTemp);
        }else if(!strcmp(p->op2,oldTemp)){
            p->op2 = strdup(newTemp);
        }
        
        p = p->suivant;
    }
}

void modifierQuad(int pos, int action){
    int i=1;
    ListQuadruplets p = LQ;
    char ch[100];
    
    while(p!=NULL){
        if(estBranchement(p)){
            int jumpLocation = atoi(p->op1);

            if(action==INSERTION){
                if(jumpLocation>=pos){
                    jumpLocation++;
                }
                qc1++;
            }else{
                if(jumpLocation>pos){
                    jumpLocation--;
                }
              qc1--;
            }
            
            sprintf(ch,"%d",jumpLocation);
            p->op1 = strdup(ch);
        }
        
        i++;
        p = p->suivant;
    }
}


int estExpressionArithmetique(ListQuadruplets p){
    return !strcmp(p->opr,"+")||!strcmp(p->opr,"-")||!strcmp(p->opr,"*")||!strcmp(p->opr,"/");
}




qdr* convertirList(ListQuadruplets LQ){

  int i=0;
    ListQuadruplets q = LQ;

    qdr* quadObj=malloc(qc*sizeof(qdr));
        while(q!=NULL){

  


      strcpy(quadObj[i].oper , q->opr);
      strcpy(quadObj[i].op1 , q->op1);
      strcpy(quadObj[i].op2 , q->op2);
      strcpy(quadObj[i].res , q->res);

      i++;
        q = q->suivant;
  }



  return quadObj;


}

void generationCodeOBJ(){


qdr* liste = convertirList(LQ);

  int i; 

  printf("****************** Le code Objet**********************\n");

  printf("CODE SEGEMENT\n");
  printf("ASSUME CS:CODE, DS:DATA\n");
  printf("MAIN :\n");
  printf("MOV AX,DATA\n");
  printf("MOV DS,AX\n");

  for(i=0;i<qc;i++){
    printf("etiq %d : ",i);

        if(strcmp(liste[i].oper,"BNE")==0){
        printf("MOV AX, %s\n",liste[i].op1);
        printf("CMP AX, %s\n",liste[i].op2);
        printf("JNE etiq %s\n",liste[i].res);}
    
        if(strcmp(liste[i].oper,"BE")==0){
        printf("MOV AX, %s\n",liste[i].op1);
        printf("CMP AX, %s\n",liste[i].op2);
        printf("JE etiq %s\n",liste[i].res);}

        if(strcmp(liste[i].oper,"BG")==0){
        printf("MOV AX, %s\n",liste[i].op1);
        printf("CMP AX, %s\n",liste[i].op2);
        printf("JG etiq %s\n",liste[i].res);}

        if(strcmp(liste[i].oper,"BL")==0){
        printf("MOV AX, %s\n",liste[i].op1);
        printf("CMP AX, %s\n",liste[i].op2);
        printf("JL etiq %s\n",liste[i].res);}
       

         if(strcmp(liste[i].oper,"BNL")==0){
        printf("MOV AX, %s\n",liste[i].op1);
        printf("CMP AX, %s\n",liste[i].op2);
        printf("JNL etiq %s\n",liste[i].res);}

         if(strcmp(liste[i].oper,"BNG")==0){
        printf("MOV AX, %s\n",liste[i].op1);
        printf("CMP AX, %s\n",liste[i].op2);
        printf("JNG etiq %s\n",liste[i].res);}

        if(strcmp(liste[i].oper,"BR")==0){
        printf("JMP AX, %s\n",liste[i].res);}
      
        if(strcmp(liste[i].oper,"+")==0){
        printf("MOV AX, %s\n",liste[i].op1);
        printf("ADD AX, %s\n",liste[i].op2);
        printf("MOV %s, AX\n",liste[i].res);}

        if(strcmp(liste[i].oper,"-")==0){
        printf("MOV AX, %s\n",liste[i].op1);
        printf("SUB AX, %s\n",liste[i].op2);
        printf("MOV %s, AX\n",liste[i].res);}

        if(strcmp(liste[i].oper,"*")==0){
        printf("MOV AX, %s\n",liste[i].op1);
        printf("MUL AX, %s\n",liste[i].op2);
        printf("MOV %s, AX\n",liste[i].res);}

        if(strcmp(liste[i].oper,"/")==0){
        printf("MOV AX, %s\n",liste[i].op1);
        printf("DIV AX, %s\n",liste[i].op2);
        printf("MOV %s, AX\n",liste[i].res);}

        if(strcmp(liste[i].oper,"=")==0){
        printf("MOV AX, %s\n",liste[i].op1);
        printf("MOV %s, AX\n",liste[i].res);}
  }

  printf("FIN :\n");
  printf("MOV AH,4CH\n");
  printf("INT 21h\n");
  printf("CODE ENDS\n");
  printf("END MAIN\n");

}








