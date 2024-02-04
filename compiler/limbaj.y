%{
#include <iostream>
#include <vector>
#include <string.h>
#include "IdList.h"
extern FILE* yyin;
extern char* yytext;
extern int yylineno;
extern int yylex();
void yyerror(const char * s);
class IdList ids;
char declIn[20];
int ind=0;
struct AST *buildAST(char *nume, struct AST *st, struct AST *dr, enum nodetype type)
{
    AST *nod = new AST; // Using new instead of malloc
    nod->name = std::string(nume);
    nod->left = st;
    nod->right = dr;
    nod->type = type;
    return nod;
}
const char *TypeOf(IdList ids, struct AST *tree,int yylineno);

float evalAST(IdList ids, struct AST *tree, int yylineno)
{
    if (tree->left == nullptr && tree->right == nullptr)
    {
        if (tree->type == IDENTIFICATOR)
        {
            std::string type = ids.getIdType(tree->name);
            if (type == "char" || type == "string")
            {
                std::cerr << "Error: Line " << yylineno << ": Variable " << tree->name << " has invalid type: " << type << std::endl;
            }
            else
            {
                if (type == "int")
                {
                    return ids.getValueByIDi(tree->name);
                }
                else if(type == "float") return ids.getValueByIDf(tree->name);
                else if(type=="bool") return ids.getValueByIDbool(tree->name);
            }
        }
        else if (tree->type == ELEM_OF_VECTOR)
        {
            std::string type = ids.getIdType(tree->name);
            if (type == "char" || type == "string")
            {
                std::cerr << "Error: Line " << yylineno << ": Variable " << tree->name << " has invalid type: " << type << std::endl;
            }
            else
            {
                if (type == "int")
                {
                    return ids.getValueByIDiV(tree->name,ind);
                }
                else if(type == "float") return ids.getValueByIDiV(tree->name,ind);
            }
        }
        else if (tree->type == NUMAR)
        {
            return std::atoi(tree->name.c_str());
        }
        else if (tree->type == NUMAR_FLOAT)
        {
            return std::atof(tree->name.c_str());
        }
        else if (tree->type == BOOL)
        {
            return std::atoi(tree->name.c_str());
        }
        return 0;
    }
    else if(tree->right == nullptr)
    {
        float val = evalAST(ids, tree->left, yylineno); 
        if (tree->type == OPERATOR)
        {
          if (tree->name == "!")
          {
            return !val;
          }
        }
    }
    else
    {
        if(TypeOf(ids,tree->left,yylineno)!="any"&&TypeOf(ids,tree->right,yylineno)!="any")
        if(TypeOf(ids,tree->left,yylineno)!=TypeOf(ids,tree->right,yylineno))
        {fprintf(stderr,"Error at line %d : different types\n",yylineno);exit(0);}
        float val_left = evalAST(ids, tree->left, yylineno);
        float val_right = evalAST(ids, tree->right, yylineno);
        if (tree->type == OPERATOR)
        {
            if (tree->name == "+")
            {
                return val_left + val_right;
            }
            else if (tree->name == "-")
            {
                return val_left - val_right;
            }
            else if (tree->name == "*")
            {
                return val_left * val_right;
            }
            else if (tree->name == "/")
            {
                if (val_right != 0)
                {
                    return val_left / val_right;
                }
                else
                {
                    std::cerr << "Error: Line " << yylineno << ": Division by zero is not allowed." << std::endl;
                }
            }
            else if(tree->name == ">")
            {
               return val_left>val_right;
            }
            else if(tree->name == "<")
            {
                return val_left<val_right;
            }
            else if(tree->name == ">=")
            {
               return val_left>=val_right;
            }
            else if(tree->name == "<=")
            {
                return val_left<=val_right;
            }
            else if(tree->name == "==")
            {
                return val_left==val_right;
            }
            else if(tree->name == "&&")
            {
                return val_left&&val_right;
            }
            else if(tree->name == "||")
            {
                return val_left||val_right;
            }
        }
    }
    return 0;
}

const char *TypeOf(IdList ids, struct AST *tree,int yylineno)
{
        if(tree->type==OPERATOR)return "any";
        if (tree->left == nullptr && tree->right == nullptr)
        {
            if (tree->type == IDENTIFICATOR)
            {
                std::string tip = ids.getIdType(tree->name);
                if (tip == "int")
                    return "int";
                if (tip == "bool")
                    return "bool";
                if (tip == "float")
                    return "float";
                if (tip == "char")
                    return "char";
                if (tip == "string")
                    return "string";
            }
            else if (tree->type == ELEM_OF_VECTOR)
            {
                std::string tip = ids.getIdType(tree->name);
                if (tip == "int")
                    return "int";
                if (tip == "bool")
                    return "bool";
                if (tip == "float")
                    return "float";
                else if (tree->type==STRING) return "string";
                else if (tree->type==CHAR)return "char";
            }
            else if (tree->type == NUMAR)
            {
                int x = evalAST(ids, tree, yylineno); // Pass ids to evalAST
                return "int";
            }
            else if (tree->type == NUMAR_FLOAT)
            {
                float x = evalAST(ids, tree, yylineno); // Pass ids to evalAST
                return "float";
            }
            else if (tree->type == BOOL)
            {
                bool x = evalAST(ids, tree, yylineno); // Pass ids to evalAST
                return "bool";
            }
        }
    return "";
}

const char *TypeOfT(IdList ids, struct AST *tree, int yylineno)
{
    if (tree->left == nullptr && tree->right == nullptr)
    {
        if (tree->type == IDENTIFICATOR || tree->type == ELEM_OF_VECTOR)
        {
            std::string tip = ids.getIdType(tree->name);
            if (tip == "int")
                return "int";
            if (tip == "bool")
                return "bool";
            if (tip == "float")
                return "float";
            if (tip == "char")
                return "char";
            if (tip == "string")
                return "string";
        }
        else if (tree->type == NUMAR)
        {
            return "int";
        }
        else if (tree->type == NUMAR_FLOAT)
        {
            return "float";
        }
        else if (tree->type == BOOL)
        {
            return "bool";
        }
        else if (tree->type==STRING) return "string";
        else if (tree->type==CHAR)return "char";
    }
    const char *leftType = TypeOfT(ids, tree->left, yylineno);
    const char *rightType = TypeOfT(ids, tree->right, yylineno);
    return leftType;
}


float Eval(IdList ids, struct AST *tree, int yylineno)
{
     return evalAST(ids, tree, yylineno); // Pass ids to evalAST
}

struct AST *cloneAST(struct AST *tree) {
    if (tree == nullptr) {
        return nullptr;
    }

    struct AST *clone = new AST;
    clone->name = tree->name;
    clone->type = tree->type;
    clone->left = cloneAST(tree->left);
    clone->right = cloneAST(tree->right);

    return clone;
}


void freeAST(struct AST *tree) {
    if (tree) {
        freeAST(tree->left);
        freeAST(tree->right);
        delete tree;
    }
}


struct ptParam
{
     int paramCount;
     string params[50];
};

%}
%union {
     char* string;
     int val;
     bool bval;
     float fval;
     struct ptParam *pparam;
     struct AST* ast;
}
%token  BGIN END ASSIGN CLASS ENDCLASS FUNC ENDFUNC RETURN CONST IF ELSE EIF FOR TRUE FALSE
%token<string> EFOR DO WHILE EWHILE  EVAL TYPEOF STR CHR
%token<fval> FLOATNR
%token<val>NR 
%token<string> ID TYPE
%type<string> param tipex
%type<pparam>list_param call_list
%type<ast> e rval
%start progr

%left OR 
%left AND
%left NOT EQ
%left LEQ GEQ '<' '>'
%left '-' '+'
%left '/' '*'

%%
progr: class globalVar globalFunc main {printf("The programme is correct!\n");}
     | main {printf("The programme is correct!\n");}
     | globalFunc main {printf("The programme is correct!\n");}
     ;

///classes

class: CLASS ID {strcpy(declIn,$2);} interior ENDCLASS { if(!ids.existsVar($2)) {
                          ids.addClass($2);
                     }
                     else {
          fprintf(stderr,"Error at line %d user defined variable %s declared more than once \n",yylineno,$2); exit(0);}
          
                    } 
     | class CLASS ID {strcpy(declIn,$3);} interior ENDCLASS { if(!ids.existsVar($3)) {
                          ids.addClass($3);
                     }
                     else {
          fprintf(stderr,"Error at line %d user defined variable %s declared more than once \n",yylineno,$3); exit(0);}
                    } 
     ;

interior: classVarDecl classFuncDecl
        | classFuncDecl classVarDecl
        | classFuncDecl
        | classVarDecl
        ;

classVarDecl: cVarDecl ';'
            | classVarDecl cVarDecl ';'

classFuncDecl: cFctDecl ';'
             | classFuncDecl cFctDecl ';'

//int size, const char *type, const char *name, int *ival, float *fval, bool *bval,
//                    const char *strval, const char *declIn

cVarDecl: TYPE ID { if(!ids.existsVar($2)) {
                          ids.addVar(false,$1,$2,0,0,false,"\0",'\0',declIn);
                     }
                    } 
            | TYPE ID '[' NR ']' { 
                    if(!ids.existsVar($2)) {
                          ids.addArray($4,$1,$2,0,0,false,"\0",declIn);
                     }
                    }  
            | CONST TYPE ID { if(!ids.existsVar($2)) {
                          ids.addVar(true,$2,$3,0,0,false,"\0",'\0',declIn);
                     }
                    } 
           ;

cFctDecl: TYPE ID '(' list_param ')' {if(!ids.existsFct($2)) ids.addFunc($1,$2,$4->paramCount,$4->params,declIn);}
          | TYPE ID '(' ')' {if(!ids.existsFct($2)) ids.addFunc($1,$2,0,nullptr,declIn);}
          ;

//global vars

globalVar: {strcpy(declIn,"global");} gVarDecl ';' 
         | globalVar gVarDecl ';' {strcpy(declIn,"global");}
         ;

gVarDecl: TYPE ID { if(!ids.existsVar($2)) {
                          ids.addVar(false,$1,$2,0,0,false,"\0",'\0',declIn);
                     }
                     else {
          fprintf(stderr,"Error at line %d user defined variable %s declared more than once \n",yylineno,$2); exit(0);}
                    } 
            | TYPE ID '[' NR ']' { 
                    if(!ids.existsVar($2)) {
                          ids.addArray($4,$1,$2,0,0,false,"\0",declIn);
                     }
                     else {
          fprintf(stderr,"Error at line %d user defined variable %s declared more than once \n",yylineno,$2); exit(0);}
                    }  
            | CONST TYPE ID { if(!ids.existsVar($2)) {
                          ids.addVar(true,$2,$3,0,0,false,"\0",'\0',declIn);
                     }
                     else {
          fprintf(stderr,"Error at line %d user defined variable %s declared more than once \n",yylineno,$3); exit(0);}
                    } 
            | TYPE ID ASSIGN rval{
                    const char* tip=TypeOfT(ids,$4,yylineno);
                    if(strcmp(tip,$1)!=0) 
                    { fprintf(stderr,"Error at line %d different data types : %s and %s \n",yylineno,$2, tip); exit(0);}
                if(!ids.existsVar($2)) {
                          float ev=Eval(ids,$4,yylineno);
                          ids.addVar(false,$1,$2,ev,ev,ev,$4->name.c_str(),$4->name[0],declIn);
                     }
                     else {
          fprintf(stderr,"Error at line %d user defined variable %s declared more than once \n",yylineno,$2); exit(0);}
                    } 
            | CONST TYPE ID ASSIGN rval{ 
                    const char* tip=TypeOfT(ids,$5,yylineno);
                    if(strcmp(tip,$2)!=0) 
                    { fprintf(stderr,"Error at line %d different data types : %s and %s \n",yylineno,$3, tip); exit(0);}

                    if(!ids.existsVar($2)) {
                          float ev=Eval(ids,$5,yylineno);
                          ids.addVar(true,$2,$3,ev,ev,ev,$5->name.c_str(),$5->name[0],declIn);
                     }
                     else {
          fprintf(stderr,"Error at line %d user defined variable %s declared more than once \n",yylineno,$3); exit(0);}
                    } 
           ;
//global funcs
globalFunc: gfuncdecl ';' {strcpy(declIn,"global");}
          | globalFunc {strcpy(declIn,"global");} gfuncdecl ';' {strcpy(declIn,"global");}
          ;
      
gfuncdecl:  TYPE ID '(' list_param ')' {if(!ids.existsFct($2)) ids.addFunc($1,$2,$4->paramCount,$4->params,declIn);free($4);}
           | TYPE ID '(' ')'  {if(!ids.existsFct($2)) ids.addFunc($1,$2,0,nullptr,declIn);}
           | TYPE ID '(' ')' FUNC {strcpy(declIn,$2);}pseudo_list RETURN rval ';' ENDFUNC {strcpy(declIn,"global");
           if(!ids.existsFct($2)) ids.addFunc($1,$2,0,nullptr,declIn);}
           | TYPE ID '(' list_param ')' FUNC {strcpy(declIn,$2);} pseudo_list RETURN rval ';' ENDFUNC {strcpy(declIn,"global");
           if(!ids.existsFct($2))ids.addFunc($1,$2,$4->paramCount,$4->params,declIn);}
           ;   
//pseudo

pseudo_list :  pseudo_statement ';' 
     | pseudo_list pseudo_statement ';'
     | pseudo_list pseudo_if
     | pseudo_list pseudo_while
     | pseudo_list pseudo_do
     | pseudo_list pseudo_for
     ;

pseudo_if: IF '(' e ')' pseudo_list EIF
  | IF '(' e ')' pseudo_list ELSE pseudo_list EIF
  ;

pseudo_for : FOR '(' TYPE ID ASSIGN rval                     { 
                    if(!ids.existsVar($4)) {
                          bool isArray = false;
                          ids.addVar(false,$3,$4,0,0,&isArray,"\0",'\0',declIn);
                     }
                    }  ';' e ';' pseudo_statement ')' pseudo_list EFOR

    | FOR '(' ID ASSIGN rval ';' e ';' pseudo_statement ')' pseudo_list EFOR
    ;

pseudo_do : DO pseudo_list WHILE '(' e ')' ';'
   ;

pseudo_while : WHILE '(' e ')' pseudo_list EWHILE
      ;

pseudo_statement: ID '[' NR ']' ASSIGN rval {if(ids.existsArr($1)==0){
          fprintf(stderr,"Error at line %d variable %s not declared\n",yylineno,$1); exit(0);}
          if(ids.getIdType($1)!=TypeOfT(ids,$6,yylineno))
          {
          fprintf(stderr,"Error at line %d variable %s does not have type %s\n",yylineno,$1,TypeOfT(ids,$6,yylineno)); exit(0);
          }
          }
        | ID ASSIGN rval {if(ids.existsVar($1)==0){
          fprintf(stderr,"Error at line %d variable %s not declared\n",yylineno,$1); exit(0);}
          if(ids.isConst($1)==0){
          if(ids.getIdType($1)!=TypeOfT(ids,$3,yylineno))
          {
          fprintf(stderr,"Error at line %d variable %s does not have type %s\n",yylineno,$1,TypeOfT(ids,$3,yylineno)); exit(0);
          }
          }
          else{
          fprintf(stderr,"Error at line %d variable %s is const\n",yylineno,$1); exit(0);}
          }
        | ID '.' ID ASSIGN rval {if (!ids.existsVar($3)|| $1!=ids.getDeclSpace($3) ){
          fprintf(stderr,"Error at line %d variable %s not declared\n",yylineno,$3); exit(0);}
          if(ids.getIdType($3)!=TypeOfT(ids,$5,yylineno))
          {
          fprintf(stderr,"Error at line %d variable %s does not have type %s\n",yylineno,$3,TypeOfT(ids,$5,yylineno)); exit(0);
          }
          }
        | ID '.' ID '(' call_list ')' {if (!ids.existsFct($3)|| $1!=ids.getDeclSpaceFct($3) ){
          fprintf(stderr,"Error at line %d variable %s not declared\n",yylineno,$1); exit(0);}
          else if(ids.paramsFct($3,$5->paramCount,$5->params)==0){
          fprintf(stderr,"Error at line %d function %s does not have the right parameters\n",
          yylineno,$1); exit(0);}
          }
        | ID '.' ID '(' ')' {if (!ids.existsFct($3)|| $1!=ids.getDeclSpaceFct($3) ){
          fprintf(stderr,"Error at line %d variable %s not declared\n",yylineno,$1); exit(0);}
          else if(ids.paramsFct($3,0,nullptr)==0){
          fprintf(stderr,"Error at line %d function %s does not have the right parameters\n",
          yylineno,$1); exit(0);}
          }
        | ID '(' call_list ')' {if(ids.existsFct($1)==0 || ids.getDeclSpaceFct($1)!="global"){
          fprintf(stderr,"Error at line %d function %s not declared\n",yylineno,$1); exit(0);}
          else if(ids.paramsFct($1,$3->paramCount,$3->params)==0){
          fprintf(stderr,"Error at line %d function %s does not have the right parameters\n",
          yylineno,$1); exit(0);}
          }
        | ID '(' ')' {if(ids.existsFct($1)==0|| ids.getDeclSpaceFct($1)!="global"){
          fprintf(stderr,"Error at line %d function %s not declared\n",yylineno,$1); exit(0);}
          else if(ids.paramsFct($1,0,nullptr)==0){
          fprintf(stderr,"Error at line %d function %s does not have the right parameters\n",
          yylineno,$1); exit(0);}
          }
        | mainDeclarations
        ;

        


//pseudo/

///main 
main : {strcpy(declIn,"main");} BGIN list END  
     ;
     
mainDeclarations :  mdecl        
	      |  mainDeclarations mdecl ';'
	      ;

mdecl     :  TYPE ID { 
                    if(!ids.existsVar($2)) {
                          bool isArray = false;
                          ids.addVar(false,$1,$2,0,0,&isArray,"\0",'\0',declIn);
                     }
                     else {
          fprintf(stderr,"Error at line %d user defined variable %s declared more than once \n",yylineno,$2); exit(0);}
                    } 
            | TYPE ID '[' NR ']' { 
                    if(!ids.existsVar($2)) {
                          ids.addArray($4,$1,$2,0,0,false,"\0",declIn);
                     }
                     else {
          fprintf(stderr,"Error at line %d user defined variable %s declared more than once \n",yylineno,$2); exit(0);}
                    }  
            | CONST TYPE ID { 
                    if(!ids.existsVar($2)) {
                          bool isArray = false;
                          ids.addVar(true,$2,$3,0,0,&isArray,"\0",'\0',declIn);
                     }
                     else {
          fprintf(stderr,"Error at line %d user defined variable %s declared more than once \n",yylineno,$3); exit(0);}
                    } 
            | TYPE ID ASSIGN rval{
                    const char* tip=TypeOfT(ids,$4,yylineno);
                    if(strcmp(tip,$1)!=0) 
                    { fprintf(stderr,"Error at line %d different data types : %s and %s \n",yylineno,$2, tip); exit(0);}
                    if(!ids.existsVar($2)) {
                          bool isArray = false;
                          float ev=Eval(ids,$4,yylineno);
                          ids.addVar(false,$1,$2,ev,ev,ev,$4->name.c_str(),$4->name[0],declIn);
                     }
                     else {
          fprintf(stderr,"Error at line %d user defined variable %s declared more than once \n",yylineno,$2); exit(0);}
                    } 
            | TYPE ID '[' NR ']' ASSIGN rval { 
                    const char* tip=TypeOfT(ids,$7,yylineno);
                    if(strcmp(tip,$1)!=0) 
                    { fprintf(stderr,"Error at line %d different data types : %s and %s \n",yylineno,$2, tip); exit(0);}

                    if(!ids.existsVar($2)) {
                          float evf=Eval(ids,$7,yylineno);
                          int evi=evf;
                          bool evb=evi;
                          ids.addArray($4,$1,$2,evi,evf,evb,$7->name.c_str(),declIn);
                     }
                     else {
          fprintf(stderr,"Error at line %d user defined variable %s declared more than once \n",yylineno,$2); exit(0);}
                    } 
            | CONST TYPE ID ASSIGN rval{ 
                    const char* tip=TypeOfT(ids,$5,yylineno);
                    if(strcmp(tip,$2)!=0) 
                    { fprintf(stderr,"Error at line %d different data types : %s and %s \n",yylineno,$3, tip); exit(0);}
                    if(!ids.existsVar($2)) {
                          bool isArray = false;
                          float ev=Eval(ids,$5,yylineno);
                          ids.addVar(true,$2,$3,ev,ev,ev,$5->name.c_str(),$5->name[0],declIn);
                     }
                     else {
          fprintf(stderr,"Error at line %d user defined variable %s declared more than once \n",yylineno,$3); exit(0);}
                    } 
           ; 


list_param : param {
                $$ = (struct ptParam*)malloc(sizeof(struct ptParam));
                $$->paramCount = 1;
                $$->params[0] = $1;
            }
            | list_param ',' param {
                $$->params[$1->paramCount] = $3;
                $$->paramCount = $1->paramCount + 1;
            }
            ;

param : TYPE ID {
            $$ = strdup($1);
        }
      ;


list :  statement ';' 
     | list statement ';'
     | list if
     | list while
     | list do
     | list for
     ;

if: IF '(' e ')' list EIF
  | IF '(' e ')' list ELSE list EIF
  ;

for : FOR '(' TYPE ID ASSIGN rval                     { 
                    if(!ids.existsVar($4)) {
                          bool isArray = false;
                          ids.addVar(false,$3,$4,0,0,&isArray,"\0",'\0',declIn);
                     }
                    }  ';' e ';' statement ')' list EFOR

    | FOR '(' ID ASSIGN rval ';' e ';' statement ')' list EFOR
    ;

do : DO list WHILE '(' e ')' ';'
   ;

while : WHILE '(' e ')' list EWHILE
      ;

statement: ID '[' NR ']' ASSIGN rval {if(ids.existsArr($1)==0){
          fprintf(stderr,"Error at line %d variable %s not declared\n",yylineno,$1); exit(0);}
          if(ids.getIdType($1)!=TypeOfT(ids,$6,yylineno))
          {
          fprintf(stderr,"Error at line %d variable %s does not have type %s\n",yylineno,$1,TypeOfT(ids,$6,yylineno)); exit(0);
          }
          float ev=Eval(ids,$6,yylineno);
          ids.updateInArr($3, $1, ids.getIdType($1), ev, ev, '\0', ev);
          }
        | ID ASSIGN rval {if(ids.existsVar($1)==0){
          fprintf(stderr,"Error at line %d variable %s not declared\n",yylineno,$1); exit(0);}
          if(ids.isConst($1)==0){
          if(ids.getIdType($1)!=TypeOfT(ids,$3,yylineno))
          {
          fprintf(stderr,"Error at line %d variable %s does not have type %s\n",yylineno,$1,TypeOfT(ids,$3,yylineno)); exit(0);
          }
          float ev=Eval(ids,$3,yylineno);
          ids.updateVar($1, ids.getIdType($1), ev, ev, $3->name[0],$3->name,ev);
          }
          else{
          fprintf(stderr,"Error at line %d variable %s is const\n",yylineno,$1); exit(0);}
          }
        | ID '.' ID ASSIGN rval {if (!ids.existsVar($3)|| $1!=ids.getDeclSpace($3) ){
          fprintf(stderr,"Error at line %d variable %s not declared\n",yylineno,$3); exit(0);}
          if(ids.getIdType($3)!=TypeOfT(ids,$5,yylineno))
          {
          fprintf(stderr,"Error at line %d variable %s does not have type %s\n",yylineno,$3,TypeOfT(ids,$5,yylineno)); exit(0);
          }
          float ev=Eval(ids,$5,yylineno);
          ids.updateVar($3, ids.getIdType($3), ev, ev, $5->name[0],$5->name,ev);
          }
        | ID '.' ID '(' call_list ')' {if (!ids.existsFct($3)|| $1!=ids.getDeclSpaceFct($3) ){
          fprintf(stderr,"Error at line %d variable %s not declared\n",yylineno,$1); exit(0);}
          else if(ids.paramsFct($3,$5->paramCount,$5->params)==0){
          fprintf(stderr,"Error at line %d function %s does not have the right parameters\n",
          yylineno,$1); exit(0);}
          }
        | ID '.' ID '(' ')' {if (!ids.existsFct($3)|| $1!=ids.getDeclSpaceFct($3) ){
          fprintf(stderr,"Error at line %d variable %s not declared\n",yylineno,$1); exit(0);}
          else if(ids.paramsFct($3,0,nullptr)==0){
          fprintf(stderr,"Error at line %d function %s does not have the right parameters\n",
          yylineno,$1); exit(0);}
          }
        | ID '(' call_list ')' {if(ids.existsFct($1)==0 || ids.getDeclSpaceFct($1)!="global"){
          fprintf(stderr,"Error at line %d function %s not declared\n",yylineno,$1); exit(0);}
          else if(ids.paramsFct($1,$3->paramCount,$3->params)==0){
          fprintf(stderr,"Error at line %d function %s does not have the right parameters\n",
          yylineno,$1); exit(0);}
          }
        | ID '(' ')' {if(ids.existsFct($1)==0|| ids.getDeclSpaceFct($1)!="global"){
          fprintf(stderr,"Error at line %d function %s not declared\n",yylineno,$1); exit(0);}
          else if(ids.paramsFct($1,0,nullptr)==0){
          fprintf(stderr,"Error at line %d function %s does not have the right parameters\n",
          yylineno,$1); exit(0);}
          }
        | mainDeclarations
        | EVAL '(' e ')' {printf("%f\n", Eval(ids,$3, yylineno));freeAST($3);}
        | TYPEOF '(' tipex ')' {printf("%s\n", $3);}
        ;

        
call_list : rval {
                $$ = (struct ptParam*)malloc(sizeof(struct ptParam));
                $$->paramCount = 1;
                $$->params[0] = TypeOf(ids,$1,yylineno);
            }
            | call_list ',' rval {
                $$->params[$1->paramCount] = TypeOf(ids,$3,yylineno);
                $$->paramCount = $1->paramCount + 1;
            }
            ;

rval: e {$$=$1;}
    ;

//TO DO si aici
e: e '*' e {if($1->type==STRING||$1->type==CHAR||$3->type==STRING||$3->type==CHAR)
   {fprintf(stderr,"Error at line : %d you cannot do string expressions\n",yylineno);exit(0);} 
   $$ = buildAST(strdup("*"), $1, $3, OPERATOR); }
  | e '/' e {if($1->type==STRING||$1->type==CHAR||$3->type==STRING||$3->type==CHAR)
   {fprintf(stderr,"Error at line : %d you cannot do string expressions\n",yylineno);exit(0);} 
   $$ = buildAST(strdup("/"), $1, $3, OPERATOR); }
  | e '+' e { if($1->type==STRING||$1->type==CHAR||$3->type==STRING||$3->type==CHAR)
   {fprintf(stderr,"Error at line : %d you cannot do string expressions\n",yylineno);exit(0);}
   $$ = buildAST(strdup("+"), $1, $3, OPERATOR); }
  | e '-' e { if($1->type==STRING||$1->type==CHAR||$3->type==STRING||$3->type==CHAR)
   {fprintf(stderr,"Error at line : %d you cannot do string expressions\n",yylineno);exit(0);}
   $$ = buildAST(strdup("-"), $1, $3, OPERATOR); }
  | e '<' e { if($1->type==STRING||$1->type==CHAR||$3->type==STRING||$3->type==CHAR)
   {fprintf(stderr,"Error at line : %d you cannot do string expressions\n",yylineno);exit(0);}
   $$ = buildAST(strdup("<"), $1, $3, OPERATOR); }
  | e '>' e { if($1->type==STRING||$1->type==CHAR||$3->type==STRING||$3->type==CHAR)
   {fprintf(stderr,"Error at line : %d you cannot do string expressions\n",yylineno);exit(0);}
   $$ = buildAST(strdup(">"), $1, $3, OPERATOR); }
  | e GEQ e { if($1->type==STRING||$1->type==CHAR||$3->type==STRING||$3->type==CHAR)
   {fprintf(stderr,"Error at line : %d you cannot do string expressions\n",yylineno);exit(0);}
   $$ = buildAST(strdup(">="), $1, $3, OPERATOR); }
  | e EQ e { if($1->type==STRING||$1->type==CHAR||$3->type==STRING||$3->type==CHAR)
   {fprintf(stderr,"Error at line : %d you cannot do string expressions\n",yylineno);exit(0);}
   $$ = buildAST(strdup("=="), $1, $3, OPERATOR); }
  | e LEQ e { if($1->type==STRING||$1->type==CHAR||$3->type==STRING||$3->type==CHAR)
   {fprintf(stderr,"Error at line : %d you cannot do string expressions\n",yylineno);exit(0);}
   $$ = buildAST(strdup("<="), $1, $3, OPERATOR); }
  | e AND e { if($1->type==STRING||$1->type==CHAR||$3->type==STRING||$3->type==CHAR)
   {fprintf(stderr,"Error at line : %d you cannot do string expressions\n",yylineno);exit(0);}
   $$ = buildAST(strdup("||"), $1, $3, OPERATOR); }
  | e OR e { if($1->type==STRING||$1->type==CHAR||$3->type==STRING||$3->type==CHAR)
   {fprintf(stderr,"Error at line : %d you cannot do string expressions\n",yylineno);exit(0);}
   $$ = buildAST(strdup("||"), $1, $3, OPERATOR); }
  | NOT e { if($2->type==STRING||$2->type==CHAR)
   {fprintf(stderr,"Error at line : %d you cannot do string expressions\n",yylineno);exit(0);}
   $$ = buildAST(strdup("!"), $2, nullptr, OPERATOR); }
  | '(' e ')' { if($2->type==STRING||$2->type==CHAR)
   {fprintf(stderr,"Error at line : %d you cannot do string expressions\n",yylineno);exit(0);}
   $$ = $2; }
  | ID {
    if (!ids.existsVar($1)) {
        fprintf(stderr, "Error at line %d: Variable %s not declared\n", yylineno, $1);
        exit(0);
    }
    $$ = buildAST($1, NULL, NULL, IDENTIFICATOR);
}
  | NR { char nr[100]; bzero(&nr, 100); sprintf(nr, "%d", $1); $$ = buildAST(nr, NULL, NULL, NUMAR); }
  | FLOATNR { char nr[100]; bzero(&nr, 100); sprintf(nr, "%f", $1); $$ = buildAST(nr, NULL, NULL, NUMAR_FLOAT); }
  | TRUE { char nr[4]; bzero(&nr, 4); sprintf(nr, "%d", 1);$$ = buildAST(nr, NULL, NULL, BOOL); }
  | FALSE { char nr[4]; bzero(&nr, 4); sprintf(nr, "%d", 0);$$ = buildAST(nr, NULL, NULL, BOOL); }
  | CHR {$$ = buildAST(strdup($1), nullptr, nullptr, CHAR);}
  | STR {$$ = buildAST(strdup($1), nullptr, nullptr, STRING);}
  | ID '.' ID {
    if (!ids.existsVar($3)|| $1!=ids.getDeclSpace($3) ) {
        fprintf(stderr, "Error at line %d: Variable %s not declared\n", yylineno, $1);
        exit(0);
    }
    $$ = buildAST($3, NULL, NULL, IDENTIFICATOR);
}
  | ID '(' call_list ')' { if(ids.existsFct($1)==0||"global"!=ids.getDeclSpaceFct($1)){
          fprintf(stderr,"Error at line %d function %s not declared\n",yylineno,$1); exit(0);}
          else if(ids.paramsFct($1,$3->paramCount,$3->params)==0){
          fprintf(stderr,"Error at line %d function %s does not have the right parameters\n",
          yylineno,$1); exit(0);}
          if(ids.getFctRetType($1)=="int")
                            $$ = buildAST(strdup("0"), NULL, NULL, NUMAR);
          if(ids.getFctRetType($1)=="char")
                            $$ = buildAST(strdup("0"), NULL, NULL, CHAR);
          if(ids.getFctRetType($1)=="string")
                            $$ = buildAST(strdup("0"), NULL, NULL, STRING);
          if(ids.getFctRetType($1)=="float")
                            $$ = buildAST(strdup("0"), NULL, NULL, NUMAR_FLOAT);
          if(ids.getFctRetType($1)=="bool")
                            $$ = buildAST(strdup("0"), NULL, NULL, BOOL);
               }
  | ID '.' ID '(' call_list ')' { if(ids.existsFct($3)==0||$1!=ids.getDeclSpaceFct($3)){
          fprintf(stderr,"Error at line %d function %s not declared\n",yylineno,$1); exit(0);}
          else if(ids.paramsFct($3,$5->paramCount,$5->params)==0){
          fprintf(stderr,"Error at line %d function %s does not have the right parameters\n",
          yylineno,$3); exit(0);}
          if(ids.getFctRetType($3)=="int")
                            $$ = buildAST(strdup("0"), NULL, NULL, NUMAR);
          if(ids.getFctRetType($3)=="char")
                            $$ = buildAST(strdup("0"), NULL, NULL, CHAR);
          if(ids.getFctRetType($3)=="string")
                            $$ = buildAST(strdup("0"), NULL, NULL, STRING);
          if(ids.getFctRetType($3)=="float")
                            $$ = buildAST(strdup("0"), NULL, NULL, NUMAR_FLOAT);
          if(ids.getFctRetType($3)=="bool")
                            $$ = buildAST(strdup("0"), NULL, NULL, BOOL);
               }
  | ID '(' ')' { {if(ids.existsFct($1)==0||"global"!=ids.getDeclSpaceFct($1)){
          fprintf(stderr,"Error at line %d function %s not declared\n",yylineno,$1); exit(0);}
          }
          if(ids.getFctRetType($1)=="int")
                            $$ = buildAST(strdup("0"), NULL, NULL, NUMAR);
          if(ids.getFctRetType($1)=="char")
                            $$ = buildAST(strdup("0"), NULL, NULL, CHAR);
          if(ids.getFctRetType($1)=="string")
                            $$ = buildAST(strdup("0"), NULL, NULL, STRING);
          if(ids.getFctRetType($1)=="float")
                            $$ = buildAST(strdup("0"), NULL, NULL, NUMAR_FLOAT);
          if(ids.getFctRetType($1)=="bool")
                            $$ = buildAST(strdup("0"), NULL, NULL, BOOL);
               }
  | ID '.' ID '(' ')' { {if(ids.existsFct($3)==0||$1!=ids.getDeclSpaceFct($3)){
          fprintf(stderr,"Error at line %d function %s not declared\n",yylineno,$1); exit(0);}
          }
          if(ids.getFctRetType($3)=="int")
                            $$ = buildAST(strdup("0"), NULL, NULL, NUMAR);
          if(ids.getFctRetType($3)=="char")
                            $$ = buildAST(strdup("0"), NULL, NULL, CHAR);
          if(ids.getFctRetType($3)=="string")
                            $$ = buildAST(strdup("0"), NULL, NULL, STRING);
          if(ids.getFctRetType($3)=="float")
                            $$ = buildAST(strdup("0"), NULL, NULL, NUMAR_FLOAT);
          if(ids.getFctRetType($3)=="bool")
                            $$ = buildAST(strdup("0"), NULL, NULL, BOOL);
               }
  | ID '[' NR ']' {
    if (!ids.existsVar($1)) {
        fprintf(stderr, "Error at line %d: Variable %s not declared\n", yylineno, $1);
        exit(0);
    }
    ind=$3;
    $$ = buildAST($1, NULL, NULL, ELEM_OF_VECTOR);
}
  ;


tipex : tipex '+' tipex {
                                    if(strcmp($1, $3))
                                    {
                                        fprintf(stderr, "Error at line %d: different data types\n", yylineno);
                                        exit(0);
                                    }
                                    $$ = $1;
                                 }
         | tipex '-' tipex {
                                    if(strcmp($1, $3))
                                    {
                                        fprintf(stderr, "Error at line %d: different data types\n", yylineno);
                                        exit(0);
                                    }
                                    $$ = $1;
                                 }
         | tipex '/' tipex {
                                    if(strcmp($1, $3))
                                    {
                                        fprintf(stderr, "Error at line %d: different data types\n", yylineno);
                                        exit(0);
                                    }
                                    $$ = $1;
                                 }
         | tipex '*' tipex {
                                    if(strcmp($1, $3))
                                    {
                                        fprintf(stderr, "Error at line %d: different data types\n", yylineno);
                                        exit(0);
                                    }
                                    $$ = $1;
                                 }
         | tipex '<' tipex {
                                    if(strcmp($1, $3))
                                    {
                                        fprintf(stderr, "Error at line %d: different data types\n", yylineno);
                                        exit(0);
                                    }
                                    $$ = $1;
                                 }
         | tipex '>' tipex {
                                    if(strcmp($1, $3))
                                    {
                                        fprintf(stderr, "Error at line %d: different data types\n", yylineno);
                                        exit(0);
                                    }
                                    $$ = $1;
                                 }
         | tipex GEQ tipex {
                                    if(strcmp($1, $3))
                                    {
                                        fprintf(stderr, "Error at line %d: different data types\n", yylineno);
                                        exit(0);
                                    }
                                    $$ = $1;
                                 }
         | tipex EQ tipex {
                                    if(strcmp($1, $3))
                                    {
                                        fprintf(stderr, "Error at line %d: different data types\n", yylineno);
                                        exit(0);
                                    }
                                    $$ = $1;
                                 }
         | tipex LEQ tipex {
                                    if(strcmp($1, $3))
                                    {
                                        fprintf(stderr, "Error at line %d: different data types\n", yylineno);
                                        exit(0);
                                    }
                                    $$ = $1;
                                 }
         | tipex AND tipex {
                                    if(strcmp($1, $3))
                                    {
                                        fprintf(stderr, "Error at line %d: different data types\n", yylineno);
                                        exit(0);
                                    }
                                    $$ = $1;
                                 }
         | tipex OR tipex {
                                    if(strcmp($1, $3))
                                    {
                                        fprintf(stderr, "Error at line %d: different data types\n", yylineno);
                                        exit(0);
                                    }
                                    $$ = $1;
                                 }
         | NOT tipex { $$ = $2; }
         | '(' tipex ')' { $$ = $2; }
         | ID { if(ids.existsVar($1) == 0){
         fprintf(stderr,"Error at line %d variable %s not declared\n",yylineno,$1);
         exit(0);
         }
         $$ = strdup(ids.getIdType($1).c_str());
         }
         | NR {$$ = strdup("int");}
         | TRUE {$$ = strdup("bool");}
         | FALSE {$$ = strdup("bool");}
         | FLOATNR {$$ = strdup("float");}
         | STR {$$ = strdup("string");}
         | CHR {$$ = strdup("char");}
         | ID '[' NR ']' { if(ids.existsVar($1) == 0){
         fprintf(stderr,"Error at line %d variable %s not declared\n",yylineno,$1);
         exit(0);
         }
         $$ = strdup(ids.getIdType($1).c_str());
         }
         | ID '(' call_list ')' { if(ids.existsFct($1)==0||"global"!=ids.getDeclSpaceFct($1)){
          fprintf(stderr,"Error at line %d function %s not declared\n",yylineno,$1); exit(0);}
          else if(ids.paramsFct($1,$3->paramCount,$3->params)==0){
          fprintf(stderr,"Error at line %d function %s does not have the right parameters\n",
          yylineno,$1); exit(0);}
          $$ = strdup(ids.getFctRetType($1).c_str()); 
          }
         | ID '.' ID {if(ids.existsVar($3)==0||$1!=ids.getDeclSpaceFct($3)){
          fprintf(stderr,"Error at line %d function %s not declared\n",yylineno,$1); exit(0);}
            $$ = strdup(ids.getFctRetType($3).c_str());
         }
         | ID '.' ID '(' call_list ')' { if(ids.existsFct($3)==0||$1!=ids.getDeclSpaceFct($3)){
          fprintf(stderr,"Error at line %d function %s not declared\n",yylineno,$1); exit(0);}
          else if(ids.paramsFct($3,$5->paramCount,$5->params)==0){
          fprintf(stderr,"Error at line %d function %s does not have the right parameters\n",
          yylineno,$1); exit(0);}
            $$ = strdup(ids.getFctRetType($3).c_str());
         }
         ;

%%
void yyerror(const char * s){
printf("error: %s at line:%d\n",s,yylineno);
}

int main(int argc, char** argv){
     yyin=fopen(argv[1],"r");
     yyparse();
     ids.printVars();
    
}