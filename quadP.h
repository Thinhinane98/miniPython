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
typedef ElementQuad* ListQuad;

typedef struct ElementQ ElementQ;
typedef ElementQ* StackQuad;

struct ElementQ{
    ListQuad quad;
    StackQuad next;
};

struct ElementQuad { 
    char* opr;           
    char* op1;           
    char* op2;           
    char* res;
    ListQuad next;
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
{ //printf("le quad num %d est ajoure avec %s\n",num_quad,val );
if (colon_quad==0)    strcpy(quad[num_quad].oper ,  val);
else if (colon_quad==1)   strcpy(quad[num_quad].op1  ,  val);
         else if (colon_quad==2)    strcpy(quad[num_quad].op2  ,   val);
                   else if (colon_quad==3)    strcpy(quad[num_quad].res  ,   val);

}
void afficher_qdr()
{
printf("*********************LesQuadruplets***********************\n");

int i;

for(i=0;i<qc+1;i++)
		{
 printf("\n %d - ( %s  ,  %s  ,  %s  ,  %s )",i,quad[i].oper,quad[i].op1,quad[i].op2,quad[i].res); 
 printf("\n---------------------------------------------------\n");
}
}


ListQuad popQuad(StackQuad* S);
void pushQuad(StackQuad* S,ListQuad quad);
ListQuad quadAfter(ListQuad p, char* opr,char* op1,char* op2,char* res);
ListQuad removeQuadAff(char* res);
ListQuad removeQuadRef( ListQuad p);
void redundantExpressions( ListQuad LQ);
void expressionPropagation(ListQuad LQ);
void simplifyCalculations(ListQuad LQ);
void variablePropagation(ListQuad LQ);
void expressionPropagation(ListQuad LQ);
int isArithmeticOperation(ListQuad p);
int isAssignationOperation(ListQuad p);
void updateQuadsFor(int pos, int action);
int getQuadPosition(ListQuad p);
int isNumber(char* ch);
void replaceTempBy(char* oldTemp, char* newTemp);
int isEmptyQuad(StackQuad S);
int isJumpOperation(ListQuad p);


ListQuad LQ = NULL;


void optimize( ListQuad LQ){

   int a=qc1;
   //while(1){
   // nbtrys++;
    redundantExpressions(LQ);
    expressionPropagation(LQ);
    simplifyCalculations(LQ);
    variablePropagation(LQ);
    

    if(!LQ) printf("pdr\n");
   // if(nbtrys==100){nbtrys=0; break;}
/*}*/}

void afficherQuadListOpt(){
  int i=0;
    ListQuad q = LQ;
    printf("********************* Les Quadruplets opt***********************\n");
    while(q!=NULL){
    printf("%d - (%s,%s,%s,%s)\n",i++,q->opr,q->op1,q->op2,q->res);
        q = q->next;
  }
  
  printf("\n");
}


void quadlist(char* opr,char* op1,char* op2,char* res){
    ListQuad newElement = (ListQuad) malloc( sizeof(ElementQuad));
    
    newElement->opr = opr;
    newElement->op1 = op1;
    newElement->op2 = op2;
    newElement->res = res;
    newElement->next = NULL;
    
    if(LQ == NULL){
        LQ=newElement;
    }else{
        ListQuad q = LQ;
        while(q->next!=NULL){
            q=q->next;
        }
        q->next = newElement;
    }
}


void tableToListe(){

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


optimize(LQ);

afficherQuadListOpt();




}


//opt



void expressionPropagation(ListQuad LQ){
    ListQuad p,q,s,r,t;
    char* temp1;
    char* temp2;
    int isConstant;
    StackQuad SQ = NULL;
    
    q=LQ;
    
    while(LQ != NULL){
        if(!isArithmeticOperation(LQ)){   
            q = LQ->next;
            LQ = LQ->next;
            continue;
        }
        
        r = LQ->next;
        isConstant = 1;
        
       
        
        
        if(r!=NULL&&isAssignationOperation(r)&&isConstant){
            p = r->next;
            
            while(p!=NULL){
                if(isAssignationOperation(p)){
                    if(!strcmp(p->res,r->res)){//On ne fait pas l'optimisation car x a changé
                        p = p->next;
                    }else if(!strcmp(p->op1,r->res)){
                        s = p;
                        while(q!=r){
                            updateQuadsFor(getQuadPosition(s->next),INSERTION);
                            s = quadAfter(s,q->opr,q->op1,q->op2,q->res);
                            q = q->next;
                        }
                            updateQuadsFor(getQuadPosition(s->next),INSERTION);
                            s = quadAfter(s,q->opr,q->op1,q->op2,q->res);
                            s->res = strdup(p->res);
                            updateQuadsFor(getQuadPosition(p),SUPPRESSION);
                            p = removeQuadRef(p);
                            LQ = p;
                            break;
                        
                    }else{
                        p = p->next;
                    }p = p->next;
                }else{
                    p = p->next;
                }
            }
            
            LQ = r->next;
        }else{
            if(r!=NULL){
                LQ = r->next;
            }else{
                LQ = NULL;
            }
        }
        
    }
}





void simplifyCalculations(ListQuad LQ){
    ListQuad p;
    while(LQ!=NULL){
        if(!strcmp(LQ->opr,"*")){
            if(isNumber(LQ->op1)&&!isNumber(LQ->op2)){
                int n = atoi(LQ->op1);
                
                if(n>1&&n<=10){
                    updateQuadsFor(getQuadPosition(LQ->next),INSERTION);
                    p = quadAfter(LQ,"+",LQ->op2,LQ->op2,LQ->res);
                    n--;
                    while(n>1){
                        updateQuadsFor(getQuadPosition(p->next),INSERTION);
                        p = quadAfter(p,"+",LQ->op2,LQ->res,LQ->res);
                        n--;
                    }
                    
                    updateQuadsFor(getQuadPosition(LQ),SUPPRESSION);
                    LQ = removeQuadRef(LQ);
                    continue;
                }else if(n==1){
                    replaceTempBy(LQ->res,LQ->op2);
                    updateQuadsFor(getQuadPosition(LQ),SUPPRESSION);
                    LQ = removeQuadRef(LQ);
                    continue;
                }
            }else if(isNumber(LQ->op2)&&!isNumber(LQ->op1)){
                int n = atoi(LQ->op2);
                
                if(n>1&&n<=10){
                    updateQuadsFor(getQuadPosition(LQ->next),INSERTION);
                    p = quadAfter(LQ,"+",LQ->op1,LQ->op1,LQ->res);
                    n--;
                    while(n>1){
                        updateQuadsFor(getQuadPosition(p->next),INSERTION);
                        p = quadAfter(p,"+",LQ->op1,LQ->res,LQ->res);
                        n--;
                    }
                    
                    updateQuadsFor(getQuadPosition(LQ),SUPPRESSION);
                    LQ = removeQuadRef(LQ);
                    continue;
                }else if(n==1){
                    replaceTempBy(LQ->res,LQ->op1);
                    updateQuadsFor(getQuadPosition(LQ),SUPPRESSION);
                    LQ = removeQuadRef(LQ);
                    continue;
                }
            }else{
                int n1 = atoi(LQ->op1);
                int n2 = atoi(LQ->op2);
                
                if(n1==1){
                    replaceTempBy(LQ->res,LQ->op2);
                    updateQuadsFor(getQuadPosition(LQ),SUPPRESSION);
                    LQ = removeQuadRef(LQ);
                    continue;
                }else if(n2==1){
                    replaceTempBy(LQ->res,LQ->op1);
                    updateQuadsFor(getQuadPosition(LQ),SUPPRESSION);
                    LQ = removeQuadRef(LQ);
                    continue;
                }else if(n1>1&&n1<=10){
                    updateQuadsFor(getQuadPosition(LQ->next),INSERTION);
                    p = quadAfter(LQ,"+",LQ->op2,LQ->op2,LQ->res);
                    n1--;
                    while(n1>1){
                        updateQuadsFor(getQuadPosition(p->next),INSERTION);
                        p = quadAfter(p,"+",LQ->op2,LQ->res,LQ->res);
                        n1--;
                    }
                    
                    updateQuadsFor(getQuadPosition(LQ),SUPPRESSION);
                    LQ = removeQuadRef(LQ);
                    continue;
                }else if(n2>1&&n2<=10){
                    updateQuadsFor(getQuadPosition(LQ->next),INSERTION);
                    p = quadAfter(LQ,"+",LQ->op2,LQ->op2,LQ->res);
                    n2--;
                    while(n2>1){
                        updateQuadsFor(getQuadPosition(p->next),INSERTION);
                        p = quadAfter(p,"+",LQ->op2,LQ->res,LQ->res);
                        n2--;
                    }
                    
                    updateQuadsFor(getQuadPosition(LQ),SUPPRESSION);
                    LQ = removeQuadRef(LQ);
                    continue;
                    
                }
                
            }
            
            LQ = LQ->next;
            
        }else if(!strcmp(LQ->opr,"+")){
            if(isNumber(LQ->op1)){
                int n = atoi(LQ->op1);
                
                if(n==0){
                    replaceTempBy(LQ->res,LQ->op2);
                    updateQuadsFor(getQuadPosition(LQ),SUPPRESSION);
                    LQ = removeQuadRef(LQ);
                    continue;
                }
            }
            if(isNumber(LQ->op2)){
                int n = atoi(LQ->op2);
                
                if(n==0){
                    replaceTempBy(LQ->res,LQ->op1);
                    updateQuadsFor(getQuadPosition(LQ),SUPPRESSION);
                    LQ = removeQuadRef(LQ);
                    continue;
                }
            }
                LQ = LQ->next;
            
        }else if(!strcmp(LQ->opr,"-")&&strcmp(LQ->op2,"")){
            if(isNumber(LQ->op2)){
                int n = atoi(LQ->op2);
                
                if(n==0){
                    replaceTempBy(LQ->res,LQ->op1);
                    updateQuadsFor(getQuadPosition(LQ),SUPPRESSION);
                    LQ = removeQuadRef(LQ);
                }else{
                    LQ = LQ->next;
                }
            }else{
                LQ = LQ->next;
            }
        }else if(!strcmp(LQ->opr,"/")){
            if(isNumber(LQ->op2)){
                int n = atoi(LQ->op2);
                
                if(n==1){
                    replaceTempBy(LQ->res,LQ->op1);
                    updateQuadsFor(getQuadPosition(LQ),SUPPRESSION);
                    LQ = removeQuadRef(LQ);
                }else{
                    LQ = LQ->next;
                }
            }else{
                LQ = LQ->next;
            }
        }else{
            LQ = LQ->next;
        }
        
    }
}

void variablePropagation(ListQuad LQ){
    ListQuad p;
    int mustPropage;
    
    while(LQ != NULL){    
        if(isAssignationOperation(LQ)){
            p = LQ->next;
            mustPropage = 0;
            while(p!=NULL&&(!isAssignationOperation(p)||strcmp(LQ->res,p->res))){
                if(!strcmp(LQ->res,p->op1)){
                    p->op1 = strdup(LQ->op1);
                    mustPropage = 1;
                }
                if(!strcmp(LQ->res,p->op2)){
                    p->op2 = strdup(LQ->op1);
                    mustPropage = 1;
                }
                
                p = p->next;
            }
            
            if(mustPropage){
                updateQuadsFor(getQuadPosition(LQ),SUPPRESSION);
                LQ = removeQuadRef(LQ);
            }else{
                LQ = LQ->next;
            }
        }else{
           LQ = LQ->next; 
        }
        
    }
}


void redundantExpressions( ListQuad LQ){
    ListQuad p,q,s,r;
    char* temp1;
    char* temp2;
    int isConstant;
    StackQuad SQ = NULL;
    
    while(LQ != NULL){    
        if(!isArithmeticOperation(LQ)){        
            LQ = LQ->next;
            continue;
        }
        
        p = LQ;
        r = p->next;
        q = r;
        
        while(r!=NULL){
            isConstant = 1;
            while(p!=NULL&&q!=NULL&&!strcmp(p->opr,q->opr)&&isArithmeticOperation(p)){
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
                pushQuad(&SQ,q);
                p = p->next;
                q = q->next;
            }
            if(p!=NULL&&q!=NULL&&isAssignationOperation(p)&&isAssignationOperation(q)&&!isConstant){ //Expressions équivalentes
                q->op1 = strdup(p->res);
                while(!isEmptyQuad(SQ)){
                    s = popQuad(&SQ);
                    updateQuadsFor(getQuadPosition(s),SUPPRESSION);
                    s = removeQuadRef(s);
                }
                p = LQ;
                r = s;
                q = r;
            }else{
                while(!isEmptyQuad(SQ)){
                    popQuad(&SQ);
                }
                p = LQ;
                r = r->next;
                q = r;
            }
        }
        
        LQ = LQ->next;
    }
}


int isNumber(char* ch){
    int i=0;
    while(i<strlen(ch)){
        if(!isdigit(ch[i])){
            return 0;
        }
        i++;
    }
    return 1; //retourne 1
}


ListQuad popQuad(StackQuad* S){
    if(!isEmptyQuad(*S)){
        StackQuad p = *S;
        ListQuad quad = p->quad;
        *S = p->next;
        free(p);
        return quad;
    }

    return NULL;
}

void pushQuad(StackQuad* S,ListQuad quad){
    StackQuad newElement = (StackQuad) malloc(sizeof(ElementQ));

    newElement->quad = quad;
    newElement->next = *S;
    *S=newElement;
}

int isEmptyQuad(StackQuad S){
    return S == NULL;
}


void setQuad(int qc,int pos,char* newValue){
    int i=0;
    ListQuad q = LQ;
    
    while(q != NULL && i<qc){
        q = q->next;
        i++;
    }
    
  switch(pos){
    case 1:
      q->opr = newValue;
      break;
    case 2:
      q->op1 = newValue;
      break;
    case 3:
      q->op2 = newValue;
      break;
    case 4:
      q->res = newValue;
      break;
  }
}

ListQuad removeQuad(char* opr, char* op1, char* op2){
    
    if(LQ!=NULL){
        if(!strcmp(LQ->opr,opr)&&!strcmp(LQ->op1,op1)&&!strcmp(LQ->op1,op2)){
            ListQuad temp = LQ;
            LQ = LQ->next;
            free(temp);
        }else{
            ListQuad p = LQ;
            ListQuad q = LQ->next;
            
            while(q!=NULL){
                if(!strcmp(LQ->opr,opr)&&!strcmp(LQ->op1,op1)&&!strcmp(LQ->op1,op2)){
                    p->next = q->next;
                    free(q);
                    break;
                }else{
                    p = q;
                    q = q->next;
                }
            }
        }
    }
    
    return LQ;
}

ListQuad removeQuadRef( ListQuad p){
    if(p!=NULL){
        if(p == LQ){
            LQ = LQ->next;
            free(p);
            return LQ;
        }else{
            ListQuad q = LQ;
            
            while(q!=NULL && q->next != p){
                q = q->next;
            }
            
            if(q!=NULL){
                q->next = p->next;
                free(p);
                return q->next;
            }
        }
    }
    
    return LQ;
}

ListQuad removeQuadAff(char* res){
    
    if(LQ!=NULL){
        if(!strcmp(LQ->res,res)){
            ListQuad temp = LQ;
            LQ = LQ->next;
            return LQ;
            free(temp);
        }else{
            ListQuad p = LQ;
            ListQuad q = LQ->next;
            
            while(q!=NULL){
                if(!strcmp(q->res,res)){
                    p->next = q->next;
                    free(q);
                    return p->next;
                }else{
                    p = q;
                    q = q->next;
                }
            }
        }
    }
    
    return LQ;
}

ListQuad quadAfter(ListQuad p, char* opr,char* op1,char* op2,char* res){
    if(p!=NULL){
        ListQuad newElement = (ListQuad) malloc(sizeof(ElementQuad));

        newElement->opr = opr;
        newElement->op1 = op1;
        newElement->op2 = op2;
        newElement->res = res;
        newElement->next = p->next;
        
        p->next = newElement;
        
        return newElement;
        
    }else{
        return NULL;
    }
}

int getQuadPosition(ListQuad p){
    int i=1;
    ListQuad q = LQ;
    
    while(q != NULL && q != p){
        i++;
        q = q->next;
    }
    
    if(q==NULL){
        return 0;
    }else{
        return i;
    }
}

void replaceTempBy(char* oldTemp, char* newTemp){
    ListQuad p = LQ;
    
    while(p!=NULL){
        if(!strcmp(p->op1,oldTemp)){
            p->op1 = strdup(newTemp);
        }else if(!strcmp(p->op2,oldTemp)){
            p->op2 = strdup(newTemp);
        }
        
        p = p->next;
    }
}

void updateQuadsFor(int pos, int action){
    int i=1;
    ListQuad p = LQ;
    char ch[100];
    
    while(p!=NULL){
        if(isJumpOperation(p)){
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
        p = p->next;
    }
}


int isArithmeticOperation(ListQuad p){
    return !strcmp(p->opr,"+")||!strcmp(p->opr,"-")||!strcmp(p->opr,"*")||!strcmp(p->opr,"/");
}

int isAssignationOperation(ListQuad p){
    return !strcmp(p->opr,"=");
}

int isJumpOperation(ListQuad p){
    return !strcmp(p->opr,"BR")||!strcmp(p->opr,"BE")||!strcmp(p->opr,"BNE")||!strcmp(p->opr,"BG")||!strcmp(p->opr,"BGE")||!strcmp(p->opr,"BL")||!strcmp(p->opr,"BLE");
}


qdr* ListeToTable(ListQuad LQ){

  int i=0;
    ListQuad q = LQ;

    qdr* quadOBJ=malloc(qc*sizeof(qdr));
        while(q!=NULL){

  


      strcpy(quadOBJ[i].oper , q->opr);
      strcpy(quadOBJ[i].op1 , q->op1);
      strcpy(quadOBJ[i].op2 , q->op2);
      strcpy(quadOBJ[i].res , q->res);

      i++;
        q = q->next;
  }



  return quadOBJ;


}

void generationCodeOBJ(){


qdr* liste = ListeToTable(LQ);

  int i; 
 // FILE *f=fopen("CODEOBJ.txt","w");
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
  //fclose(f);
}








