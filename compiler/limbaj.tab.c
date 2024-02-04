/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "limbaj.y"

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


#line 348 "limbaj.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "limbaj.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_BGIN = 3,                       /* BGIN  */
  YYSYMBOL_END = 4,                        /* END  */
  YYSYMBOL_ASSIGN = 5,                     /* ASSIGN  */
  YYSYMBOL_CLASS = 6,                      /* CLASS  */
  YYSYMBOL_ENDCLASS = 7,                   /* ENDCLASS  */
  YYSYMBOL_FUNC = 8,                       /* FUNC  */
  YYSYMBOL_ENDFUNC = 9,                    /* ENDFUNC  */
  YYSYMBOL_RETURN = 10,                    /* RETURN  */
  YYSYMBOL_CONST = 11,                     /* CONST  */
  YYSYMBOL_IF = 12,                        /* IF  */
  YYSYMBOL_ELSE = 13,                      /* ELSE  */
  YYSYMBOL_EIF = 14,                       /* EIF  */
  YYSYMBOL_FOR = 15,                       /* FOR  */
  YYSYMBOL_TRUE = 16,                      /* TRUE  */
  YYSYMBOL_FALSE = 17,                     /* FALSE  */
  YYSYMBOL_EFOR = 18,                      /* EFOR  */
  YYSYMBOL_DO = 19,                        /* DO  */
  YYSYMBOL_WHILE = 20,                     /* WHILE  */
  YYSYMBOL_EWHILE = 21,                    /* EWHILE  */
  YYSYMBOL_EVAL = 22,                      /* EVAL  */
  YYSYMBOL_TYPEOF = 23,                    /* TYPEOF  */
  YYSYMBOL_STR = 24,                       /* STR  */
  YYSYMBOL_CHR = 25,                       /* CHR  */
  YYSYMBOL_FLOATNR = 26,                   /* FLOATNR  */
  YYSYMBOL_NR = 27,                        /* NR  */
  YYSYMBOL_ID = 28,                        /* ID  */
  YYSYMBOL_TYPE = 29,                      /* TYPE  */
  YYSYMBOL_OR = 30,                        /* OR  */
  YYSYMBOL_AND = 31,                       /* AND  */
  YYSYMBOL_NOT = 32,                       /* NOT  */
  YYSYMBOL_EQ = 33,                        /* EQ  */
  YYSYMBOL_LEQ = 34,                       /* LEQ  */
  YYSYMBOL_GEQ = 35,                       /* GEQ  */
  YYSYMBOL_36_ = 36,                       /* '<'  */
  YYSYMBOL_37_ = 37,                       /* '>'  */
  YYSYMBOL_38_ = 38,                       /* '-'  */
  YYSYMBOL_39_ = 39,                       /* '+'  */
  YYSYMBOL_40_ = 40,                       /* '/'  */
  YYSYMBOL_41_ = 41,                       /* '*'  */
  YYSYMBOL_42_ = 42,                       /* ';'  */
  YYSYMBOL_43_ = 43,                       /* '['  */
  YYSYMBOL_44_ = 44,                       /* ']'  */
  YYSYMBOL_45_ = 45,                       /* '('  */
  YYSYMBOL_46_ = 46,                       /* ')'  */
  YYSYMBOL_47_ = 47,                       /* '.'  */
  YYSYMBOL_48_ = 48,                       /* ','  */
  YYSYMBOL_YYACCEPT = 49,                  /* $accept  */
  YYSYMBOL_progr = 50,                     /* progr  */
  YYSYMBOL_class = 51,                     /* class  */
  YYSYMBOL_52_1 = 52,                      /* $@1  */
  YYSYMBOL_53_2 = 53,                      /* $@2  */
  YYSYMBOL_interior = 54,                  /* interior  */
  YYSYMBOL_classVarDecl = 55,              /* classVarDecl  */
  YYSYMBOL_classFuncDecl = 56,             /* classFuncDecl  */
  YYSYMBOL_cVarDecl = 57,                  /* cVarDecl  */
  YYSYMBOL_cFctDecl = 58,                  /* cFctDecl  */
  YYSYMBOL_globalVar = 59,                 /* globalVar  */
  YYSYMBOL_60_3 = 60,                      /* $@3  */
  YYSYMBOL_gVarDecl = 61,                  /* gVarDecl  */
  YYSYMBOL_globalFunc = 62,                /* globalFunc  */
  YYSYMBOL_63_4 = 63,                      /* $@4  */
  YYSYMBOL_gfuncdecl = 64,                 /* gfuncdecl  */
  YYSYMBOL_65_5 = 65,                      /* $@5  */
  YYSYMBOL_66_6 = 66,                      /* $@6  */
  YYSYMBOL_pseudo_list = 67,               /* pseudo_list  */
  YYSYMBOL_pseudo_if = 68,                 /* pseudo_if  */
  YYSYMBOL_pseudo_for = 69,                /* pseudo_for  */
  YYSYMBOL_70_7 = 70,                      /* $@7  */
  YYSYMBOL_pseudo_do = 71,                 /* pseudo_do  */
  YYSYMBOL_pseudo_while = 72,              /* pseudo_while  */
  YYSYMBOL_pseudo_statement = 73,          /* pseudo_statement  */
  YYSYMBOL_main = 74,                      /* main  */
  YYSYMBOL_75_8 = 75,                      /* $@8  */
  YYSYMBOL_mainDeclarations = 76,          /* mainDeclarations  */
  YYSYMBOL_mdecl = 77,                     /* mdecl  */
  YYSYMBOL_list_param = 78,                /* list_param  */
  YYSYMBOL_param = 79,                     /* param  */
  YYSYMBOL_list = 80,                      /* list  */
  YYSYMBOL_if = 81,                        /* if  */
  YYSYMBOL_for = 82,                       /* for  */
  YYSYMBOL_83_9 = 83,                      /* $@9  */
  YYSYMBOL_do = 84,                        /* do  */
  YYSYMBOL_while = 85,                     /* while  */
  YYSYMBOL_statement = 86,                 /* statement  */
  YYSYMBOL_call_list = 87,                 /* call_list  */
  YYSYMBOL_rval = 88,                      /* rval  */
  YYSYMBOL_e = 89,                         /* e  */
  YYSYMBOL_tipex = 90                      /* tipex  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   866

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  49
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  42
/* YYNRULES -- Number of rules.  */
#define YYNRULES  148
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  372

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   290


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      45,    46,    41,    39,    48,    38,    47,    40,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    42,
      36,     2,    37,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    43,     2,    44,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   303,   303,   304,   305,   310,   310,   317,   317,   325,
     326,   327,   328,   331,   332,   334,   335,   340,   344,   349,
     355,   356,   361,   361,   362,   365,   371,   378,   384,   395,
     409,   410,   410,   413,   414,   415,   415,   417,   417,   422,
     423,   424,   425,   426,   427,   430,   431,   434,   434,   441,
     444,   447,   450,   457,   468,   475,   481,   487,   493,   499,
     508,   508,   511,   512,   515,   523,   530,   538,   550,   564,
     579,   584,   590,   596,   597,   598,   599,   600,   601,   604,
     605,   608,   608,   615,   618,   621,   624,   633,   646,   655,
     661,   667,   673,   679,   680,   681,   685,   690,   696,   700,
     703,   706,   709,   712,   715,   718,   721,   724,   727,   730,
     733,   736,   739,   746,   747,   748,   749,   750,   751,   752,
     759,   775,   791,   805,   819,   830,   838,   846,   854,   862,
     870,   878,   886,   894,   902,   910,   918,   919,   920,   926,
     927,   928,   929,   930,   931,   932,   938,   945,   949
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "BGIN", "END",
  "ASSIGN", "CLASS", "ENDCLASS", "FUNC", "ENDFUNC", "RETURN", "CONST",
  "IF", "ELSE", "EIF", "FOR", "TRUE", "FALSE", "EFOR", "DO", "WHILE",
  "EWHILE", "EVAL", "TYPEOF", "STR", "CHR", "FLOATNR", "NR", "ID", "TYPE",
  "OR", "AND", "NOT", "EQ", "LEQ", "GEQ", "'<'", "'>'", "'-'", "'+'",
  "'/'", "'*'", "';'", "'['", "']'", "'('", "')'", "'.'", "','", "$accept",
  "progr", "class", "$@1", "$@2", "interior", "classVarDecl",
  "classFuncDecl", "cVarDecl", "cFctDecl", "globalVar", "$@3", "gVarDecl",
  "globalFunc", "$@4", "gfuncdecl", "$@5", "$@6", "pseudo_list",
  "pseudo_if", "pseudo_for", "$@7", "pseudo_do", "pseudo_while",
  "pseudo_statement", "main", "$@8", "mainDeclarations", "mdecl",
  "list_param", "param", "list", "if", "for", "$@9", "do", "while",
  "statement", "call_list", "rval", "e", "tipex", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-195)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-61)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      31,    -5,     1,    32,     4,    33,    14,  -195,    74,  -195,
      17,  -195,    63,    11,    55,    54,  -195,  -195,     2,    56,
      47,  -195,    69,    82,   104,    33,   128,   118,   137,   166,
     126,   146,    -4,   192,    57,  -195,   305,   152,   195,   198,
     221,    56,    56,   193,   194,   210,   233,   -20,  -195,    56,
     216,     7,  -195,  -195,     6,  -195,  -195,   217,   367,   381,
     367,   227,   141,   220,    12,   215,  -195,   222,   229,     2,
     230,  -195,  -195,  -195,  -195,   223,  -195,   238,   102,  -195,
     248,   237,    59,   243,  -195,  -195,  -195,  -195,   278,   259,
     282,   286,   367,   266,   289,  -195,  -195,  -195,  -195,  -195,
    -195,    95,   367,   367,   585,  -195,  -195,  -195,  -195,  -195,
    -195,   107,   381,   381,   599,  -195,   777,   252,  -195,   105,
    -195,     0,   367,   270,  -195,   367,    67,   508,   367,  -195,
    -195,   271,    53,   272,  -195,   273,  -195,    50,  -195,  -195,
    -195,   367,  -195,   260,   367,   276,   172,   279,    66,   613,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,  -195,   281,   367,   284,   825,   627,   381,   381,   381,
     381,   381,   381,   381,   381,   381,   381,   381,  -195,   301,
    -195,   367,   367,   205,  -195,   269,   641,   309,   287,   265,
     655,   277,  -195,   113,   291,   283,    -1,   404,   280,    57,
      50,  -195,  -195,  -195,   295,  -195,   129,   297,  -195,   788,
     808,    66,   142,   142,   142,   142,    83,    83,  -195,  -195,
     296,   144,   306,  -195,   799,   817,   825,   232,   232,   232,
     232,   123,   123,  -195,  -195,   367,  -195,  -195,  -195,   155,
     345,     2,   367,   347,   367,     2,  -195,  -195,   367,   327,
     236,   328,   367,   310,   312,    50,   313,  -195,  -195,  -195,
    -195,   317,  -195,   410,  -195,  -195,   321,  -195,  -195,   367,
    -195,  -195,   367,   429,   320,   367,   669,   444,  -195,   319,
    -195,   159,     9,   322,   367,   156,   584,   367,  -195,   367,
    -195,   162,   165,  -195,     2,  -195,   367,  -195,    52,  -195,
     360,  -195,   367,   344,   364,   683,   369,   349,   330,   697,
     336,  -195,  -195,   463,   725,   337,  -195,   367,  -195,  -195,
     168,  -195,    50,   367,   375,   367,    50,   372,  -195,     2,
     367,  -195,  -195,   527,   340,   367,   711,   533,  -195,   339,
     738,    50,  -195,   367,  -195,     5,  -195,     2,     2,   552,
     751,   346,  -195,   478,   341,  -195,    50,   367,  -195,     2,
     350,   764,   493,    50,    50,  -195,   558,   354,  -195,    50,
     573,  -195
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      60,     0,     0,     0,    22,    31,     0,     3,     0,     5,
       0,     1,     0,     0,     0,     0,     4,    30,     0,     0,
       0,     7,     0,     0,     0,    31,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    62,     0,     0,     0,     0,
       0,    12,    11,     0,     0,     0,    34,     0,    70,     0,
       0,    25,    24,     2,    25,    23,    32,     0,     0,     0,
       0,     0,     0,     0,    64,     0,    61,     0,     0,     0,
       0,    75,    78,    77,    76,     0,    73,     0,    17,     6,
       9,     0,    10,     0,    13,    15,    72,    35,    33,     0,
       0,    27,     0,     0,    66,   115,   116,   118,   117,   114,
     113,   112,     0,     0,     0,   140,   141,   143,   144,   142,
     139,   138,     0,     0,     0,    87,    98,     0,    92,     0,
      96,     0,     0,     0,    63,     0,     0,     0,     0,    74,
      19,     0,     0,     0,    14,     0,    16,     0,    37,    71,
       8,     0,    28,     0,     0,     0,     0,     0,   110,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,     0,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
      91,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,    21,     0,     0,    17,     0,     0,     0,    59,
       0,    29,    26,    69,     0,   122,     0,   119,   111,   109,
     108,   106,   107,   105,   103,   104,   102,   101,   100,    99,
       0,     0,   147,   137,   135,   134,   132,   133,   131,   129,
     130,   126,   125,   127,   128,     0,    97,    88,    90,     0,
      65,     0,     0,     0,     0,     0,    18,    20,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    41,    44,    43,
      42,     0,    39,     0,   124,   120,     0,   145,   146,     0,
      86,    89,     0,     0,     0,     0,     0,     0,    53,     0,
      58,     0,     0,     0,     0,     0,     0,     0,    40,     0,
     123,     0,     0,    68,     0,    79,     0,    81,     0,    85,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   121,   148,     0,     0,     0,    84,     0,    54,    56,
       0,    36,     0,     0,     0,     0,     0,     0,    80,     0,
       0,    52,    55,     0,     0,     0,     0,     0,    38,     0,
       0,     0,    45,     0,    47,     0,    51,     0,     0,     0,
       0,     0,    50,     0,     0,    46,     0,     0,    83,     0,
       0,     0,     0,     0,     0,    82,     0,     0,    49,     0,
       0,    48
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -195,  -195,  -195,  -195,  -195,   352,   361,   363,   -34,   -23,
    -195,  -195,   388,   373,  -195,   395,  -195,  -195,  -129,  -195,
    -195,  -195,  -195,  -195,  -194,    10,  -195,   -18,   -25,   285,
     329,   -16,  -195,  -195,  -195,  -195,  -195,   -30,  -125,   -33,
     -38,   -40
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,    19,    49,    40,    41,    42,    43,    44,
      13,    14,    24,     5,    15,     6,   137,   200,   337,   257,
     258,   351,   259,   260,   198,     7,     8,   199,    35,    47,
      48,   277,    71,    72,   315,    73,    74,    37,   119,   120,
     116,   114
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      34,    60,    36,   261,   248,   182,    75,    81,   197,    65,
      12,    92,    92,    29,   302,    16,    29,   122,    34,    83,
     104,   206,    22,     9,    30,    31,    88,   115,    89,    10,
      32,    33,    11,   196,    33,    53,   -60,     1,   221,    61,
      23,    62,   249,    63,   250,   183,   251,   352,    81,    93,
      93,    34,    20,   127,   303,   123,    17,    83,   239,   142,
       2,    29,    20,    29,   148,   149,    22,    38,    29,   261,
      38,   263,   165,   166,    30,    31,    45,    18,   196,    33,
      32,    33,    45,     2,    26,    39,    33,   186,   135,   184,
     190,    21,   261,    46,   316,   187,   188,    75,    50,   192,
     153,   154,   155,   156,   157,   158,   159,   160,   201,    34,
      51,   203,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   159,   160,   281,   286,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   145,   261,
     146,   291,   147,   261,   292,   131,    52,   132,   236,   237,
     162,   180,   163,   181,   164,   261,    54,    95,    96,   247,
      55,    89,   360,   176,   177,    97,    98,    99,   100,   101,
     367,    58,   261,   102,    65,   265,   261,   181,   320,    56,
     157,   158,   159,   160,   306,   307,   103,   118,    95,    96,
     268,    59,   181,   333,    76,    57,    97,    98,    99,   100,
     101,   271,   270,   181,   102,   301,   276,   181,   311,   274,
     181,   312,   349,   181,   332,   278,   181,   103,   205,   283,
      64,    95,    96,    34,    77,   273,    78,    34,    79,    97,
      98,    99,   100,   101,   366,    84,    85,   102,    86,   293,
     370,    87,   297,    75,    91,    94,   305,    75,   121,   309,
     103,   238,    95,    96,   117,    34,   310,   124,   314,    34,
      97,    98,    99,   100,   101,   129,   130,   125,   102,   318,
     174,   175,   176,   177,   126,   128,    34,   133,   313,   134,
      34,   103,   280,    75,   331,   136,   138,   336,    45,   140,
     334,   141,   340,   143,   144,    34,   179,   185,   191,   339,
     194,   195,   344,   204,   202,   350,   235,   207,   220,    66,
     244,    34,   222,   240,   242,   243,    29,    67,   354,   361,
      68,   246,   262,    75,    69,    70,   131,    30,    31,    34,
      34,   353,    75,    32,    33,    34,   132,    95,    96,   264,
     267,    34,   266,   362,    34,    97,    98,    99,   100,   101,
     272,   269,   275,   102,   279,   284,   282,   285,   287,   288,
      95,    96,   296,   300,   304,   317,   103,   290,    97,    98,
      99,   100,   101,   321,   323,   325,   102,   324,   327,   330,
     335,   338,   343,    95,    96,   347,    25,   359,   357,   103,
     319,    97,    98,    99,   100,   101,   363,   105,   106,   102,
     369,    90,    27,    82,    80,   107,   108,   109,   110,   111,
      28,     0,   103,   112,   252,    29,   253,   193,   139,   254,
     289,    29,   253,   255,   256,   254,   113,     0,     0,   255,
     256,     0,   196,    33,     0,     0,     0,     0,   196,    33,
      29,    67,   294,   295,    68,     0,     0,     0,    69,    70,
       0,    30,    31,     0,     0,    29,    67,    32,    33,    68,
       0,     0,     0,    69,    70,   299,    30,    31,     0,     0,
       0,     0,    32,    33,    29,    67,     0,   328,    68,     0,
       0,     0,    69,    70,     0,    30,    31,     0,     0,    29,
      67,    32,    33,    68,     0,     0,   358,    69,    70,     0,
      30,    31,     0,     0,    29,    67,    32,    33,    68,     0,
       0,   365,    69,    70,     0,    30,    31,     0,     0,    29,
      67,    32,    33,    68,     0,     0,     0,    69,   189,     0,
      30,    31,     0,     0,     0,     0,    32,    33,    29,   253,
     341,   342,   254,     0,    29,   253,   255,   256,   254,     0,
       0,     0,   255,   256,   346,   196,    33,     0,     0,     0,
       0,   196,    33,    29,   253,     0,   355,   254,     0,    29,
     253,   255,   256,   254,     0,     0,   368,   255,   256,     0,
     196,    33,     0,     0,    29,   253,   196,    33,   254,     0,
       0,   371,   255,   256,     0,    29,   253,     0,     0,   254,
       0,   196,    33,   255,   308,     0,     0,     0,     0,     0,
       0,     0,   196,    33,     0,   150,   151,     0,   152,   153,
     154,   155,   156,   157,   158,   159,   160,     0,     0,   167,
     168,   161,   169,   170,   171,   172,   173,   174,   175,   176,
     177,     0,     0,   150,   151,   178,   152,   153,   154,   155,
     156,   157,   158,   159,   160,     0,     0,   167,   168,   208,
     169,   170,   171,   172,   173,   174,   175,   176,   177,     0,
       0,   150,   151,   223,   152,   153,   154,   155,   156,   157,
     158,   159,   160,     0,     0,   150,   151,   241,   152,   153,
     154,   155,   156,   157,   158,   159,   160,     0,     0,   150,
     151,   245,   152,   153,   154,   155,   156,   157,   158,   159,
     160,     0,     0,   150,   151,   298,   152,   153,   154,   155,
     156,   157,   158,   159,   160,     0,     0,   150,   151,   322,
     152,   153,   154,   155,   156,   157,   158,   159,   160,     0,
       0,   150,   151,   326,   152,   153,   154,   155,   156,   157,
     158,   159,   160,     0,     0,   150,   151,   345,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   329,   150,   151,
       0,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     348,   150,   151,     0,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   356,   150,   151,     0,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   364,   150,   151,     0,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   151,
       0,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     168,     0,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   170,
     171,   172,   173,   174,   175,   176,   177
};

static const yytype_int16 yycheck[] =
{
      18,     5,    18,   197,     5,     5,    36,    41,   137,    34,
       6,     5,     5,    11,     5,     5,    11,     5,    36,    42,
      58,   146,    11,    28,    22,    23,    46,    60,    48,    28,
      28,    29,     0,    28,    29,    25,     3,     6,   163,    43,
      29,    45,    43,    47,    45,    45,    47,    42,    82,    43,
      43,    69,    45,    69,    45,    43,    42,    80,   183,    92,
      29,    11,    45,    11,   102,   103,    11,    11,    11,   263,
      11,   200,   112,   113,    22,    23,    29,     3,    28,    29,
      28,    29,    29,    29,    29,    29,    29,   125,    29,   122,
     128,    28,   286,    46,    42,    28,    29,   127,    29,    46,
      34,    35,    36,    37,    38,    39,    40,    41,   141,   127,
      28,   144,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,    40,    41,   250,   255,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,    43,   333,
      45,   266,    47,   337,   269,    43,    42,    45,   181,   182,
      43,    46,    45,    48,    47,   349,    28,    16,    17,    46,
      42,    48,   356,    40,    41,    24,    25,    26,    27,    28,
     364,    45,   366,    32,   199,    46,   370,    48,   303,    42,
      38,    39,    40,    41,    28,    29,    45,    46,    16,    17,
      46,    45,    48,   322,    42,    29,    24,    25,    26,    27,
      28,    46,   235,    48,    32,    46,   244,    48,    46,   242,
      48,    46,   341,    48,    46,   248,    48,    45,    46,   252,
      28,    16,    17,   241,    29,   241,    28,   245,     7,    24,
      25,    26,    27,    28,   363,    42,    42,    32,    28,   272,
     369,     8,   275,   273,    28,    28,   284,   277,    28,   287,
      45,    46,    16,    17,    27,   273,   289,    42,   296,   277,
      24,    25,    26,    27,    28,    42,    28,    45,    32,   302,
      38,    39,    40,    41,    45,    45,   294,    29,   294,    42,
     298,    45,    46,   313,   317,    42,     8,   325,    29,     7,
     323,     5,   330,    27,     5,   313,    44,    27,    27,   329,
      28,    28,   335,    27,    44,   343,     5,    28,    27,     4,
      45,   329,    28,    44,     5,    28,    11,    12,   348,   357,
      15,    44,    42,   353,    19,    20,    43,    22,    23,   347,
     348,   347,   362,    28,    29,   353,    45,    16,    17,    44,
      44,   359,    45,   359,   362,    24,    25,    26,    27,    28,
       5,    45,     5,    32,    27,    45,    28,    45,    45,    42,
      16,    17,    42,    44,    42,     5,    45,    46,    24,    25,
      26,    27,    28,     9,     5,    45,    32,    28,    42,    42,
       5,     9,    42,    16,    17,    46,    13,    46,    42,    45,
      46,    24,    25,    26,    27,    28,    46,    16,    17,    32,
      46,    49,    14,    42,    41,    24,    25,    26,    27,    28,
      15,    -1,    45,    32,    10,    11,    12,   132,    89,    15,
      10,    11,    12,    19,    20,    15,    45,    -1,    -1,    19,
      20,    -1,    28,    29,    -1,    -1,    -1,    -1,    28,    29,
      11,    12,    13,    14,    15,    -1,    -1,    -1,    19,    20,
      -1,    22,    23,    -1,    -1,    11,    12,    28,    29,    15,
      -1,    -1,    -1,    19,    20,    21,    22,    23,    -1,    -1,
      -1,    -1,    28,    29,    11,    12,    -1,    14,    15,    -1,
      -1,    -1,    19,    20,    -1,    22,    23,    -1,    -1,    11,
      12,    28,    29,    15,    -1,    -1,    18,    19,    20,    -1,
      22,    23,    -1,    -1,    11,    12,    28,    29,    15,    -1,
      -1,    18,    19,    20,    -1,    22,    23,    -1,    -1,    11,
      12,    28,    29,    15,    -1,    -1,    -1,    19,    20,    -1,
      22,    23,    -1,    -1,    -1,    -1,    28,    29,    11,    12,
      13,    14,    15,    -1,    11,    12,    19,    20,    15,    -1,
      -1,    -1,    19,    20,    21,    28,    29,    -1,    -1,    -1,
      -1,    28,    29,    11,    12,    -1,    14,    15,    -1,    11,
      12,    19,    20,    15,    -1,    -1,    18,    19,    20,    -1,
      28,    29,    -1,    -1,    11,    12,    28,    29,    15,    -1,
      -1,    18,    19,    20,    -1,    11,    12,    -1,    -1,    15,
      -1,    28,    29,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    29,    -1,    30,    31,    -1,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    -1,    -1,    30,
      31,    46,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    -1,    30,    31,    46,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    -1,    30,    31,    46,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      -1,    30,    31,    46,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    -1,    30,    31,    46,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    -1,    -1,    30,
      31,    46,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    -1,    30,    31,    46,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    -1,    30,    31,    46,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      -1,    30,    31,    46,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    -1,    30,    31,    46,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    30,    31,
      -1,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    30,    31,    -1,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    30,    31,    -1,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    30,    31,    -1,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    31,
      -1,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      31,    -1,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    34,
      35,    36,    37,    38,    39,    40,    41
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     6,    29,    50,    51,    62,    64,    74,    75,    28,
      28,     0,     6,    59,    60,    63,    74,    42,     3,    52,
      45,    28,    11,    29,    61,    62,    29,    61,    64,    11,
      22,    23,    28,    29,    76,    77,    80,    86,    11,    29,
      54,    55,    56,    57,    58,    29,    46,    78,    79,    53,
      29,    28,    42,    74,    28,    42,    42,    29,    45,    45,
       5,    43,    45,    47,    28,    77,     4,    12,    15,    19,
      20,    81,    82,    84,    85,    86,    42,    29,    28,     7,
      56,    57,    55,    58,    42,    42,    28,     8,    46,    48,
      54,    28,     5,    43,    28,    16,    17,    24,    25,    26,
      27,    28,    32,    45,    89,    16,    17,    24,    25,    26,
      27,    28,    32,    45,    90,    88,    89,    27,    46,    87,
      88,    28,     5,    43,    42,    45,    45,    80,    45,    42,
      28,    43,    45,    29,    42,    29,    42,    65,     8,    79,
       7,     5,    88,    27,     5,    43,    45,    47,    89,    89,
      30,    31,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    46,    43,    45,    47,    90,    90,    30,    31,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    46,    44,
      46,    48,     5,    45,    88,    27,    89,    28,    29,    20,
      89,    27,    46,    78,    28,    28,    28,    67,    73,    76,
      66,    88,    44,    88,    27,    46,    87,    28,    46,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      27,    87,    28,    46,    90,    90,    90,    90,    90,    90,
      90,    90,    90,    90,    90,     5,    88,    88,    46,    87,
      44,    46,     5,    28,    45,    46,    44,    46,     5,    43,
      45,    47,    10,    12,    15,    19,    20,    68,    69,    71,
      72,    73,    42,    67,    44,    46,    45,    44,    46,    45,
      88,    46,     5,    80,    88,     5,    89,    80,    88,    27,
      46,    87,    28,    88,    45,    45,    67,    45,    42,    10,
      46,    87,    87,    88,    13,    14,    42,    88,    46,    21,
      44,    46,     5,    45,    42,    89,    28,    29,    20,    89,
      88,    46,    46,    80,    89,    83,    42,     5,    88,    46,
      87,     9,    46,     5,    28,    45,    46,    42,    14,    42,
      42,    88,    46,    67,    88,     5,    89,    67,     9,    86,
      89,    13,    14,    42,    88,    46,    21,    46,    42,    67,
      89,    70,    42,    80,    86,    14,    42,    42,    18,    46,
      73,    89,    80,    46,    42,    18,    67,    73,    18,    46,
      67,    18
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    49,    50,    50,    50,    52,    51,    53,    51,    54,
      54,    54,    54,    55,    55,    56,    56,    57,    57,    57,
      58,    58,    60,    59,    59,    61,    61,    61,    61,    61,
      62,    63,    62,    64,    64,    65,    64,    66,    64,    67,
      67,    67,    67,    67,    67,    68,    68,    70,    69,    69,
      71,    72,    73,    73,    73,    73,    73,    73,    73,    73,
      75,    74,    76,    76,    77,    77,    77,    77,    77,    77,
      78,    78,    79,    80,    80,    80,    80,    80,    80,    81,
      81,    83,    82,    82,    84,    85,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    87,    87,    88,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    90,    90,    90,    90,    90,
      90,    90,    90,    90,    90,    90,    90,    90,    90,    90,
      90,    90,    90,    90,    90,    90,    90,    90,    90
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     4,     1,     2,     0,     5,     0,     6,     2,
       2,     1,     1,     2,     3,     2,     3,     2,     5,     3,
       5,     4,     0,     3,     3,     2,     5,     3,     4,     5,
       2,     0,     4,     5,     4,     0,    11,     0,    12,     2,
       3,     2,     2,     2,     2,     6,     8,     0,    14,    12,
       7,     6,     6,     3,     5,     6,     5,     4,     3,     1,
       0,     4,     1,     3,     2,     5,     3,     4,     7,     5,
       1,     3,     2,     2,     3,     2,     2,     2,     2,     6,
       8,     0,    14,    12,     7,     6,     6,     3,     5,     6,
       5,     4,     3,     1,     4,     4,     1,     3,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     3,     1,     1,     1,     1,     1,     1,     1,     3,
       4,     6,     3,     5,     4,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     3,     1,     1,
       1,     1,     1,     1,     1,     4,     4,     3,     6
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* progr: class globalVar globalFunc main  */
#line 303 "limbaj.y"
                                       {printf("The programme is correct!\n");}
#line 1759 "limbaj.tab.c"
    break;

  case 3: /* progr: main  */
#line 304 "limbaj.y"
            {printf("The programme is correct!\n");}
#line 1765 "limbaj.tab.c"
    break;

  case 4: /* progr: globalFunc main  */
#line 305 "limbaj.y"
                       {printf("The programme is correct!\n");}
#line 1771 "limbaj.tab.c"
    break;

  case 5: /* $@1: %empty  */
#line 310 "limbaj.y"
                {strcpy(declIn,(yyvsp[0].string));}
#line 1777 "limbaj.tab.c"
    break;

  case 6: /* class: CLASS ID $@1 interior ENDCLASS  */
#line 310 "limbaj.y"
                                                       { if(!ids.existsVar((yyvsp[-3].string))) {
                          ids.addClass((yyvsp[-3].string));
                     }
                     else {
          fprintf(stderr,"Error at line %d user defined variable %s declared more than once \n",yylineno,(yyvsp[-3].string)); exit(0);}
          
                    }
#line 1789 "limbaj.tab.c"
    break;

  case 7: /* $@2: %empty  */
#line 317 "limbaj.y"
                      {strcpy(declIn,(yyvsp[0].string));}
#line 1795 "limbaj.tab.c"
    break;

  case 8: /* class: class CLASS ID $@2 interior ENDCLASS  */
#line 317 "limbaj.y"
                                                             { if(!ids.existsVar((yyvsp[-3].string))) {
                          ids.addClass((yyvsp[-3].string));
                     }
                     else {
          fprintf(stderr,"Error at line %d user defined variable %s declared more than once \n",yylineno,(yyvsp[-3].string)); exit(0);}
                    }
#line 1806 "limbaj.tab.c"
    break;

  case 17: /* cVarDecl: TYPE ID  */
#line 340 "limbaj.y"
                  { if(!ids.existsVar((yyvsp[0].string))) {
                          ids.addVar(false,(yyvsp[-1].string),(yyvsp[0].string),0,0,false,"\0",'\0',declIn);
                     }
                    }
#line 1815 "limbaj.tab.c"
    break;

  case 18: /* cVarDecl: TYPE ID '[' NR ']'  */
#line 344 "limbaj.y"
                                 { 
                    if(!ids.existsVar((yyvsp[-3].string))) {
                          ids.addArray((yyvsp[-1].val),(yyvsp[-4].string),(yyvsp[-3].string),0,0,false,"\0",declIn);
                     }
                    }
#line 1825 "limbaj.tab.c"
    break;

  case 19: /* cVarDecl: CONST TYPE ID  */
#line 349 "limbaj.y"
                            { if(!ids.existsVar((yyvsp[-1].string))) {
                          ids.addVar(true,(yyvsp[-1].string),(yyvsp[0].string),0,0,false,"\0",'\0',declIn);
                     }
                    }
#line 1834 "limbaj.tab.c"
    break;

  case 20: /* cFctDecl: TYPE ID '(' list_param ')'  */
#line 355 "limbaj.y"
                                     {if(!ids.existsFct((yyvsp[-3].string))) ids.addFunc((yyvsp[-4].string),(yyvsp[-3].string),(yyvsp[-1].pparam)->paramCount,(yyvsp[-1].pparam)->params,declIn);}
#line 1840 "limbaj.tab.c"
    break;

  case 21: /* cFctDecl: TYPE ID '(' ')'  */
#line 356 "limbaj.y"
                            {if(!ids.existsFct((yyvsp[-2].string))) ids.addFunc((yyvsp[-3].string),(yyvsp[-2].string),0,nullptr,declIn);}
#line 1846 "limbaj.tab.c"
    break;

  case 22: /* $@3: %empty  */
#line 361 "limbaj.y"
           {strcpy(declIn,"global");}
#line 1852 "limbaj.tab.c"
    break;

  case 24: /* globalVar: globalVar gVarDecl ';'  */
#line 362 "limbaj.y"
                                  {strcpy(declIn,"global");}
#line 1858 "limbaj.tab.c"
    break;

  case 25: /* gVarDecl: TYPE ID  */
#line 365 "limbaj.y"
                  { if(!ids.existsVar((yyvsp[0].string))) {
                          ids.addVar(false,(yyvsp[-1].string),(yyvsp[0].string),0,0,false,"\0",'\0',declIn);
                     }
                     else {
          fprintf(stderr,"Error at line %d user defined variable %s declared more than once \n",yylineno,(yyvsp[0].string)); exit(0);}
                    }
#line 1869 "limbaj.tab.c"
    break;

  case 26: /* gVarDecl: TYPE ID '[' NR ']'  */
#line 371 "limbaj.y"
                                 { 
                    if(!ids.existsVar((yyvsp[-3].string))) {
                          ids.addArray((yyvsp[-1].val),(yyvsp[-4].string),(yyvsp[-3].string),0,0,false,"\0",declIn);
                     }
                     else {
          fprintf(stderr,"Error at line %d user defined variable %s declared more than once \n",yylineno,(yyvsp[-3].string)); exit(0);}
                    }
#line 1881 "limbaj.tab.c"
    break;

  case 27: /* gVarDecl: CONST TYPE ID  */
#line 378 "limbaj.y"
                            { if(!ids.existsVar((yyvsp[-1].string))) {
                          ids.addVar(true,(yyvsp[-1].string),(yyvsp[0].string),0,0,false,"\0",'\0',declIn);
                     }
                     else {
          fprintf(stderr,"Error at line %d user defined variable %s declared more than once \n",yylineno,(yyvsp[0].string)); exit(0);}
                    }
#line 1892 "limbaj.tab.c"
    break;

  case 28: /* gVarDecl: TYPE ID ASSIGN rval  */
#line 384 "limbaj.y"
                                 {
                    const char* tip=TypeOfT(ids,(yyvsp[0].ast),yylineno);
                    if(strcmp(tip,(yyvsp[-3].string))!=0) 
                    { fprintf(stderr,"Error at line %d different data types : %s and %s \n",yylineno,(yyvsp[-2].string), tip); exit(0);}
                if(!ids.existsVar((yyvsp[-2].string))) {
                          float ev=Eval(ids,(yyvsp[0].ast),yylineno);
                          ids.addVar(false,(yyvsp[-3].string),(yyvsp[-2].string),ev,ev,ev,(yyvsp[0].ast)->name.c_str(),(yyvsp[0].ast)->name[0],declIn);
                     }
                     else {
          fprintf(stderr,"Error at line %d user defined variable %s declared more than once \n",yylineno,(yyvsp[-2].string)); exit(0);}
                    }
#line 1908 "limbaj.tab.c"
    break;

  case 29: /* gVarDecl: CONST TYPE ID ASSIGN rval  */
#line 395 "limbaj.y"
                                       { 
                    const char* tip=TypeOfT(ids,(yyvsp[0].ast),yylineno);
                    if(strcmp(tip,(yyvsp[-3].string))!=0) 
                    { fprintf(stderr,"Error at line %d different data types : %s and %s \n",yylineno,(yyvsp[-2].string), tip); exit(0);}

                    if(!ids.existsVar((yyvsp[-3].string))) {
                          float ev=Eval(ids,(yyvsp[0].ast),yylineno);
                          ids.addVar(true,(yyvsp[-3].string),(yyvsp[-2].string),ev,ev,ev,(yyvsp[0].ast)->name.c_str(),(yyvsp[0].ast)->name[0],declIn);
                     }
                     else {
          fprintf(stderr,"Error at line %d user defined variable %s declared more than once \n",yylineno,(yyvsp[-2].string)); exit(0);}
                    }
#line 1925 "limbaj.tab.c"
    break;

  case 30: /* globalFunc: gfuncdecl ';'  */
#line 409 "limbaj.y"
                          {strcpy(declIn,"global");}
#line 1931 "limbaj.tab.c"
    break;

  case 31: /* $@4: %empty  */
#line 410 "limbaj.y"
                       {strcpy(declIn,"global");}
#line 1937 "limbaj.tab.c"
    break;

  case 32: /* globalFunc: globalFunc $@4 gfuncdecl ';'  */
#line 410 "limbaj.y"
                                                                {strcpy(declIn,"global");}
#line 1943 "limbaj.tab.c"
    break;

  case 33: /* gfuncdecl: TYPE ID '(' list_param ')'  */
#line 413 "limbaj.y"
                                       {if(!ids.existsFct((yyvsp[-3].string))) ids.addFunc((yyvsp[-4].string),(yyvsp[-3].string),(yyvsp[-1].pparam)->paramCount,(yyvsp[-1].pparam)->params,declIn);free((yyvsp[-1].pparam));}
#line 1949 "limbaj.tab.c"
    break;

  case 34: /* gfuncdecl: TYPE ID '(' ')'  */
#line 414 "limbaj.y"
                              {if(!ids.existsFct((yyvsp[-2].string))) ids.addFunc((yyvsp[-3].string),(yyvsp[-2].string),0,nullptr,declIn);}
#line 1955 "limbaj.tab.c"
    break;

  case 35: /* $@5: %empty  */
#line 415 "limbaj.y"
                                  {strcpy(declIn,(yyvsp[-3].string));}
#line 1961 "limbaj.tab.c"
    break;

  case 36: /* gfuncdecl: TYPE ID '(' ')' FUNC $@5 pseudo_list RETURN rval ';' ENDFUNC  */
#line 415 "limbaj.y"
                                                                                          {strcpy(declIn,"global");
           if(!ids.existsFct((yyvsp[-9].string))) ids.addFunc((yyvsp[-10].string),(yyvsp[-9].string),0,nullptr,declIn);}
#line 1968 "limbaj.tab.c"
    break;

  case 37: /* $@6: %empty  */
#line 417 "limbaj.y"
                                             {strcpy(declIn,(yyvsp[-4].string));}
#line 1974 "limbaj.tab.c"
    break;

  case 38: /* gfuncdecl: TYPE ID '(' list_param ')' FUNC $@6 pseudo_list RETURN rval ';' ENDFUNC  */
#line 417 "limbaj.y"
                                                                                                      {strcpy(declIn,"global");
           if(!ids.existsFct((yyvsp[-10].string)))ids.addFunc((yyvsp[-11].string),(yyvsp[-10].string),(yyvsp[-8].pparam)->paramCount,(yyvsp[-8].pparam)->params,declIn);}
#line 1981 "limbaj.tab.c"
    break;

  case 47: /* $@7: %empty  */
#line 434 "limbaj.y"
                                                             { 
                    if(!ids.existsVar((yyvsp[-2].string))) {
                          bool isArray = false;
                          ids.addVar(false,(yyvsp[-3].string),(yyvsp[-2].string),0,0,&isArray,"\0",'\0',declIn);
                     }
                    }
#line 1992 "limbaj.tab.c"
    break;

  case 52: /* pseudo_statement: ID '[' NR ']' ASSIGN rval  */
#line 450 "limbaj.y"
                                            {if(ids.existsArr((yyvsp[-5].string))==0){
          fprintf(stderr,"Error at line %d variable %s not declared\n",yylineno,(yyvsp[-5].string)); exit(0);}
          if(ids.getIdType((yyvsp[-5].string))!=TypeOfT(ids,(yyvsp[0].ast),yylineno))
          {
          fprintf(stderr,"Error at line %d variable %s does not have type %s\n",yylineno,(yyvsp[-5].string),TypeOfT(ids,(yyvsp[0].ast),yylineno)); exit(0);
          }
          }
#line 2004 "limbaj.tab.c"
    break;

  case 53: /* pseudo_statement: ID ASSIGN rval  */
#line 457 "limbaj.y"
                         {if(ids.existsVar((yyvsp[-2].string))==0){
          fprintf(stderr,"Error at line %d variable %s not declared\n",yylineno,(yyvsp[-2].string)); exit(0);}
          if(ids.isConst((yyvsp[-2].string))==0){
          if(ids.getIdType((yyvsp[-2].string))!=TypeOfT(ids,(yyvsp[0].ast),yylineno))
          {
          fprintf(stderr,"Error at line %d variable %s does not have type %s\n",yylineno,(yyvsp[-2].string),TypeOfT(ids,(yyvsp[0].ast),yylineno)); exit(0);
          }
          }
          else{
          fprintf(stderr,"Error at line %d variable %s is const\n",yylineno,(yyvsp[-2].string)); exit(0);}
          }
#line 2020 "limbaj.tab.c"
    break;

  case 54: /* pseudo_statement: ID '.' ID ASSIGN rval  */
#line 468 "limbaj.y"
                                {if (!ids.existsVar((yyvsp[-2].string))|| (yyvsp[-4].string)!=ids.getDeclSpace((yyvsp[-2].string)) ){
          fprintf(stderr,"Error at line %d variable %s not declared\n",yylineno,(yyvsp[-2].string)); exit(0);}
          if(ids.getIdType((yyvsp[-2].string))!=TypeOfT(ids,(yyvsp[0].ast),yylineno))
          {
          fprintf(stderr,"Error at line %d variable %s does not have type %s\n",yylineno,(yyvsp[-2].string),TypeOfT(ids,(yyvsp[0].ast),yylineno)); exit(0);
          }
          }
#line 2032 "limbaj.tab.c"
    break;

  case 55: /* pseudo_statement: ID '.' ID '(' call_list ')'  */
#line 475 "limbaj.y"
                                      {if (!ids.existsFct((yyvsp[-3].string))|| (yyvsp[-5].string)!=ids.getDeclSpaceFct((yyvsp[-3].string)) ){
          fprintf(stderr,"Error at line %d variable %s not declared\n",yylineno,(yyvsp[-5].string)); exit(0);}
          else if(ids.paramsFct((yyvsp[-3].string),(yyvsp[-1].pparam)->paramCount,(yyvsp[-1].pparam)->params)==0){
          fprintf(stderr,"Error at line %d function %s does not have the right parameters\n",
          yylineno,(yyvsp[-5].string)); exit(0);}
          }
#line 2043 "limbaj.tab.c"
    break;

  case 56: /* pseudo_statement: ID '.' ID '(' ')'  */
#line 481 "limbaj.y"
                            {if (!ids.existsFct((yyvsp[-2].string))|| (yyvsp[-4].string)!=ids.getDeclSpaceFct((yyvsp[-2].string)) ){
          fprintf(stderr,"Error at line %d variable %s not declared\n",yylineno,(yyvsp[-4].string)); exit(0);}
          else if(ids.paramsFct((yyvsp[-2].string),0,nullptr)==0){
          fprintf(stderr,"Error at line %d function %s does not have the right parameters\n",
          yylineno,(yyvsp[-4].string)); exit(0);}
          }
#line 2054 "limbaj.tab.c"
    break;

  case 57: /* pseudo_statement: ID '(' call_list ')'  */
#line 487 "limbaj.y"
                               {if(ids.existsFct((yyvsp[-3].string))==0 || ids.getDeclSpaceFct((yyvsp[-3].string))!="global"){
          fprintf(stderr,"Error at line %d function %s not declared\n",yylineno,(yyvsp[-3].string)); exit(0);}
          else if(ids.paramsFct((yyvsp[-3].string),(yyvsp[-1].pparam)->paramCount,(yyvsp[-1].pparam)->params)==0){
          fprintf(stderr,"Error at line %d function %s does not have the right parameters\n",
          yylineno,(yyvsp[-3].string)); exit(0);}
          }
#line 2065 "limbaj.tab.c"
    break;

  case 58: /* pseudo_statement: ID '(' ')'  */
#line 493 "limbaj.y"
                     {if(ids.existsFct((yyvsp[-2].string))==0|| ids.getDeclSpaceFct((yyvsp[-2].string))!="global"){
          fprintf(stderr,"Error at line %d function %s not declared\n",yylineno,(yyvsp[-2].string)); exit(0);}
          else if(ids.paramsFct((yyvsp[-2].string),0,nullptr)==0){
          fprintf(stderr,"Error at line %d function %s does not have the right parameters\n",
          yylineno,(yyvsp[-2].string)); exit(0);}
          }
#line 2076 "limbaj.tab.c"
    break;

  case 60: /* $@8: %empty  */
#line 508 "limbaj.y"
       {strcpy(declIn,"main");}
#line 2082 "limbaj.tab.c"
    break;

  case 64: /* mdecl: TYPE ID  */
#line 515 "limbaj.y"
                     { 
                    if(!ids.existsVar((yyvsp[0].string))) {
                          bool isArray = false;
                          ids.addVar(false,(yyvsp[-1].string),(yyvsp[0].string),0,0,&isArray,"\0",'\0',declIn);
                     }
                     else {
          fprintf(stderr,"Error at line %d user defined variable %s declared more than once \n",yylineno,(yyvsp[0].string)); exit(0);}
                    }
#line 2095 "limbaj.tab.c"
    break;

  case 65: /* mdecl: TYPE ID '[' NR ']'  */
#line 523 "limbaj.y"
                                 { 
                    if(!ids.existsVar((yyvsp[-3].string))) {
                          ids.addArray((yyvsp[-1].val),(yyvsp[-4].string),(yyvsp[-3].string),0,0,false,"\0",declIn);
                     }
                     else {
          fprintf(stderr,"Error at line %d user defined variable %s declared more than once \n",yylineno,(yyvsp[-3].string)); exit(0);}
                    }
#line 2107 "limbaj.tab.c"
    break;

  case 66: /* mdecl: CONST TYPE ID  */
#line 530 "limbaj.y"
                            { 
                    if(!ids.existsVar((yyvsp[-1].string))) {
                          bool isArray = false;
                          ids.addVar(true,(yyvsp[-1].string),(yyvsp[0].string),0,0,&isArray,"\0",'\0',declIn);
                     }
                     else {
          fprintf(stderr,"Error at line %d user defined variable %s declared more than once \n",yylineno,(yyvsp[0].string)); exit(0);}
                    }
#line 2120 "limbaj.tab.c"
    break;

  case 67: /* mdecl: TYPE ID ASSIGN rval  */
#line 538 "limbaj.y"
                                 {
                    const char* tip=TypeOfT(ids,(yyvsp[0].ast),yylineno);
                    if(strcmp(tip,(yyvsp[-3].string))!=0) 
                    { fprintf(stderr,"Error at line %d different data types : %s and %s \n",yylineno,(yyvsp[-2].string), tip); exit(0);}
                    if(!ids.existsVar((yyvsp[-2].string))) {
                          bool isArray = false;
                          float ev=Eval(ids,(yyvsp[0].ast),yylineno);
                          ids.addVar(false,(yyvsp[-3].string),(yyvsp[-2].string),ev,ev,ev,(yyvsp[0].ast)->name.c_str(),(yyvsp[0].ast)->name[0],declIn);
                     }
                     else {
          fprintf(stderr,"Error at line %d user defined variable %s declared more than once \n",yylineno,(yyvsp[-2].string)); exit(0);}
                    }
#line 2137 "limbaj.tab.c"
    break;

  case 68: /* mdecl: TYPE ID '[' NR ']' ASSIGN rval  */
#line 550 "limbaj.y"
                                             { 
                    const char* tip=TypeOfT(ids,(yyvsp[0].ast),yylineno);
                    if(strcmp(tip,(yyvsp[-6].string))!=0) 
                    { fprintf(stderr,"Error at line %d different data types : %s and %s \n",yylineno,(yyvsp[-5].string), tip); exit(0);}

                    if(!ids.existsVar((yyvsp[-5].string))) {
                          float evf=Eval(ids,(yyvsp[0].ast),yylineno);
                          int evi=evf;
                          bool evb=evi;
                          ids.addArray((yyvsp[-3].val),(yyvsp[-6].string),(yyvsp[-5].string),evi,evf,evb,(yyvsp[0].ast)->name.c_str(),declIn);
                     }
                     else {
          fprintf(stderr,"Error at line %d user defined variable %s declared more than once \n",yylineno,(yyvsp[-5].string)); exit(0);}
                    }
#line 2156 "limbaj.tab.c"
    break;

  case 69: /* mdecl: CONST TYPE ID ASSIGN rval  */
#line 564 "limbaj.y"
                                       { 
                    const char* tip=TypeOfT(ids,(yyvsp[0].ast),yylineno);
                    if(strcmp(tip,(yyvsp[-3].string))!=0) 
                    { fprintf(stderr,"Error at line %d different data types : %s and %s \n",yylineno,(yyvsp[-2].string), tip); exit(0);}
                    if(!ids.existsVar((yyvsp[-3].string))) {
                          bool isArray = false;
                          float ev=Eval(ids,(yyvsp[0].ast),yylineno);
                          ids.addVar(true,(yyvsp[-3].string),(yyvsp[-2].string),ev,ev,ev,(yyvsp[0].ast)->name.c_str(),(yyvsp[0].ast)->name[0],declIn);
                     }
                     else {
          fprintf(stderr,"Error at line %d user defined variable %s declared more than once \n",yylineno,(yyvsp[-2].string)); exit(0);}
                    }
#line 2173 "limbaj.tab.c"
    break;

  case 70: /* list_param: param  */
#line 579 "limbaj.y"
                   {
                (yyval.pparam) = (struct ptParam*)malloc(sizeof(struct ptParam));
                (yyval.pparam)->paramCount = 1;
                (yyval.pparam)->params[0] = (yyvsp[0].string);
            }
#line 2183 "limbaj.tab.c"
    break;

  case 71: /* list_param: list_param ',' param  */
#line 584 "limbaj.y"
                                   {
                (yyval.pparam)->params[(yyvsp[-2].pparam)->paramCount] = (yyvsp[0].string);
                (yyval.pparam)->paramCount = (yyvsp[-2].pparam)->paramCount + 1;
            }
#line 2192 "limbaj.tab.c"
    break;

  case 72: /* param: TYPE ID  */
#line 590 "limbaj.y"
                {
            (yyval.string) = strdup((yyvsp[-1].string));
        }
#line 2200 "limbaj.tab.c"
    break;

  case 81: /* $@9: %empty  */
#line 608 "limbaj.y"
                                                      { 
                    if(!ids.existsVar((yyvsp[-2].string))) {
                          bool isArray = false;
                          ids.addVar(false,(yyvsp[-3].string),(yyvsp[-2].string),0,0,&isArray,"\0",'\0',declIn);
                     }
                    }
#line 2211 "limbaj.tab.c"
    break;

  case 86: /* statement: ID '[' NR ']' ASSIGN rval  */
#line 624 "limbaj.y"
                                     {if(ids.existsArr((yyvsp[-5].string))==0){
          fprintf(stderr,"Error at line %d variable %s not declared\n",yylineno,(yyvsp[-5].string)); exit(0);}
          if(ids.getIdType((yyvsp[-5].string))!=TypeOfT(ids,(yyvsp[0].ast),yylineno))
          {
          fprintf(stderr,"Error at line %d variable %s does not have type %s\n",yylineno,(yyvsp[-5].string),TypeOfT(ids,(yyvsp[0].ast),yylineno)); exit(0);
          }
          float ev=Eval(ids,(yyvsp[0].ast),yylineno);
          ids.updateInArr((yyvsp[-3].val), (yyvsp[-5].string), ids.getIdType((yyvsp[-5].string)), ev, ev, '\0', ev);
          }
#line 2225 "limbaj.tab.c"
    break;

  case 87: /* statement: ID ASSIGN rval  */
#line 633 "limbaj.y"
                         {if(ids.existsVar((yyvsp[-2].string))==0){
          fprintf(stderr,"Error at line %d variable %s not declared\n",yylineno,(yyvsp[-2].string)); exit(0);}
          if(ids.isConst((yyvsp[-2].string))==0){
          if(ids.getIdType((yyvsp[-2].string))!=TypeOfT(ids,(yyvsp[0].ast),yylineno))
          {
          fprintf(stderr,"Error at line %d variable %s does not have type %s\n",yylineno,(yyvsp[-2].string),TypeOfT(ids,(yyvsp[0].ast),yylineno)); exit(0);
          }
          float ev=Eval(ids,(yyvsp[0].ast),yylineno);
          ids.updateVar((yyvsp[-2].string), ids.getIdType((yyvsp[-2].string)), ev, ev, (yyvsp[0].ast)->name[0],(yyvsp[0].ast)->name,ev);
          }
          else{
          fprintf(stderr,"Error at line %d variable %s is const\n",yylineno,(yyvsp[-2].string)); exit(0);}
          }
#line 2243 "limbaj.tab.c"
    break;

  case 88: /* statement: ID '.' ID ASSIGN rval  */
#line 646 "limbaj.y"
                                {if (!ids.existsVar((yyvsp[-2].string))|| (yyvsp[-4].string)!=ids.getDeclSpace((yyvsp[-2].string)) ){
          fprintf(stderr,"Error at line %d variable %s not declared\n",yylineno,(yyvsp[-2].string)); exit(0);}
          if(ids.getIdType((yyvsp[-2].string))!=TypeOfT(ids,(yyvsp[0].ast),yylineno))
          {
          fprintf(stderr,"Error at line %d variable %s does not have type %s\n",yylineno,(yyvsp[-2].string),TypeOfT(ids,(yyvsp[0].ast),yylineno)); exit(0);
          }
          float ev=Eval(ids,(yyvsp[0].ast),yylineno);
          ids.updateVar((yyvsp[-2].string), ids.getIdType((yyvsp[-2].string)), ev, ev, (yyvsp[0].ast)->name[0],(yyvsp[0].ast)->name,ev);
          }
#line 2257 "limbaj.tab.c"
    break;

  case 89: /* statement: ID '.' ID '(' call_list ')'  */
#line 655 "limbaj.y"
                                      {if (!ids.existsFct((yyvsp[-3].string))|| (yyvsp[-5].string)!=ids.getDeclSpaceFct((yyvsp[-3].string)) ){
          fprintf(stderr,"Error at line %d variable %s not declared\n",yylineno,(yyvsp[-5].string)); exit(0);}
          else if(ids.paramsFct((yyvsp[-3].string),(yyvsp[-1].pparam)->paramCount,(yyvsp[-1].pparam)->params)==0){
          fprintf(stderr,"Error at line %d function %s does not have the right parameters\n",
          yylineno,(yyvsp[-5].string)); exit(0);}
          }
#line 2268 "limbaj.tab.c"
    break;

  case 90: /* statement: ID '.' ID '(' ')'  */
#line 661 "limbaj.y"
                            {if (!ids.existsFct((yyvsp[-2].string))|| (yyvsp[-4].string)!=ids.getDeclSpaceFct((yyvsp[-2].string)) ){
          fprintf(stderr,"Error at line %d variable %s not declared\n",yylineno,(yyvsp[-4].string)); exit(0);}
          else if(ids.paramsFct((yyvsp[-2].string),0,nullptr)==0){
          fprintf(stderr,"Error at line %d function %s does not have the right parameters\n",
          yylineno,(yyvsp[-4].string)); exit(0);}
          }
#line 2279 "limbaj.tab.c"
    break;

  case 91: /* statement: ID '(' call_list ')'  */
#line 667 "limbaj.y"
                               {if(ids.existsFct((yyvsp[-3].string))==0 || ids.getDeclSpaceFct((yyvsp[-3].string))!="global"){
          fprintf(stderr,"Error at line %d function %s not declared\n",yylineno,(yyvsp[-3].string)); exit(0);}
          else if(ids.paramsFct((yyvsp[-3].string),(yyvsp[-1].pparam)->paramCount,(yyvsp[-1].pparam)->params)==0){
          fprintf(stderr,"Error at line %d function %s does not have the right parameters\n",
          yylineno,(yyvsp[-3].string)); exit(0);}
          }
#line 2290 "limbaj.tab.c"
    break;

  case 92: /* statement: ID '(' ')'  */
#line 673 "limbaj.y"
                     {if(ids.existsFct((yyvsp[-2].string))==0|| ids.getDeclSpaceFct((yyvsp[-2].string))!="global"){
          fprintf(stderr,"Error at line %d function %s not declared\n",yylineno,(yyvsp[-2].string)); exit(0);}
          else if(ids.paramsFct((yyvsp[-2].string),0,nullptr)==0){
          fprintf(stderr,"Error at line %d function %s does not have the right parameters\n",
          yylineno,(yyvsp[-2].string)); exit(0);}
          }
#line 2301 "limbaj.tab.c"
    break;

  case 94: /* statement: EVAL '(' e ')'  */
#line 680 "limbaj.y"
                         {printf("%f\n", Eval(ids,(yyvsp[-1].ast), yylineno));freeAST((yyvsp[-1].ast));}
#line 2307 "limbaj.tab.c"
    break;

  case 95: /* statement: TYPEOF '(' tipex ')'  */
#line 681 "limbaj.y"
                               {printf("%s\n", (yyvsp[-1].string));}
#line 2313 "limbaj.tab.c"
    break;

  case 96: /* call_list: rval  */
#line 685 "limbaj.y"
                 {
                (yyval.pparam) = (struct ptParam*)malloc(sizeof(struct ptParam));
                (yyval.pparam)->paramCount = 1;
                (yyval.pparam)->params[0] = TypeOf(ids,(yyvsp[0].ast),yylineno);
            }
#line 2323 "limbaj.tab.c"
    break;

  case 97: /* call_list: call_list ',' rval  */
#line 690 "limbaj.y"
                                 {
                (yyval.pparam)->params[(yyvsp[-2].pparam)->paramCount] = TypeOf(ids,(yyvsp[0].ast),yylineno);
                (yyval.pparam)->paramCount = (yyvsp[-2].pparam)->paramCount + 1;
            }
#line 2332 "limbaj.tab.c"
    break;

  case 98: /* rval: e  */
#line 696 "limbaj.y"
        {(yyval.ast)=(yyvsp[0].ast);}
#line 2338 "limbaj.tab.c"
    break;

  case 99: /* e: e '*' e  */
#line 700 "limbaj.y"
           {if((yyvsp[-2].ast)->type==STRING||(yyvsp[-2].ast)->type==CHAR||(yyvsp[0].ast)->type==STRING||(yyvsp[0].ast)->type==CHAR)
   {fprintf(stderr,"Error at line : %d you cannot do string expressions\n",yylineno);exit(0);} 
   (yyval.ast) = buildAST(strdup("*"), (yyvsp[-2].ast), (yyvsp[0].ast), OPERATOR); }
#line 2346 "limbaj.tab.c"
    break;

  case 100: /* e: e '/' e  */
#line 703 "limbaj.y"
            {if((yyvsp[-2].ast)->type==STRING||(yyvsp[-2].ast)->type==CHAR||(yyvsp[0].ast)->type==STRING||(yyvsp[0].ast)->type==CHAR)
   {fprintf(stderr,"Error at line : %d you cannot do string expressions\n",yylineno);exit(0);} 
   (yyval.ast) = buildAST(strdup("/"), (yyvsp[-2].ast), (yyvsp[0].ast), OPERATOR); }
#line 2354 "limbaj.tab.c"
    break;

  case 101: /* e: e '+' e  */
#line 706 "limbaj.y"
            { if((yyvsp[-2].ast)->type==STRING||(yyvsp[-2].ast)->type==CHAR||(yyvsp[0].ast)->type==STRING||(yyvsp[0].ast)->type==CHAR)
   {fprintf(stderr,"Error at line : %d you cannot do string expressions\n",yylineno);exit(0);}
   (yyval.ast) = buildAST(strdup("+"), (yyvsp[-2].ast), (yyvsp[0].ast), OPERATOR); }
#line 2362 "limbaj.tab.c"
    break;

  case 102: /* e: e '-' e  */
#line 709 "limbaj.y"
            { if((yyvsp[-2].ast)->type==STRING||(yyvsp[-2].ast)->type==CHAR||(yyvsp[0].ast)->type==STRING||(yyvsp[0].ast)->type==CHAR)
   {fprintf(stderr,"Error at line : %d you cannot do string expressions\n",yylineno);exit(0);}
   (yyval.ast) = buildAST(strdup("-"), (yyvsp[-2].ast), (yyvsp[0].ast), OPERATOR); }
#line 2370 "limbaj.tab.c"
    break;

  case 103: /* e: e '<' e  */
#line 712 "limbaj.y"
            { if((yyvsp[-2].ast)->type==STRING||(yyvsp[-2].ast)->type==CHAR||(yyvsp[0].ast)->type==STRING||(yyvsp[0].ast)->type==CHAR)
   {fprintf(stderr,"Error at line : %d you cannot do string expressions\n",yylineno);exit(0);}
   (yyval.ast) = buildAST(strdup("<"), (yyvsp[-2].ast), (yyvsp[0].ast), OPERATOR); }
#line 2378 "limbaj.tab.c"
    break;

  case 104: /* e: e '>' e  */
#line 715 "limbaj.y"
            { if((yyvsp[-2].ast)->type==STRING||(yyvsp[-2].ast)->type==CHAR||(yyvsp[0].ast)->type==STRING||(yyvsp[0].ast)->type==CHAR)
   {fprintf(stderr,"Error at line : %d you cannot do string expressions\n",yylineno);exit(0);}
   (yyval.ast) = buildAST(strdup(">"), (yyvsp[-2].ast), (yyvsp[0].ast), OPERATOR); }
#line 2386 "limbaj.tab.c"
    break;

  case 105: /* e: e GEQ e  */
#line 718 "limbaj.y"
            { if((yyvsp[-2].ast)->type==STRING||(yyvsp[-2].ast)->type==CHAR||(yyvsp[0].ast)->type==STRING||(yyvsp[0].ast)->type==CHAR)
   {fprintf(stderr,"Error at line : %d you cannot do string expressions\n",yylineno);exit(0);}
   (yyval.ast) = buildAST(strdup(">="), (yyvsp[-2].ast), (yyvsp[0].ast), OPERATOR); }
#line 2394 "limbaj.tab.c"
    break;

  case 106: /* e: e EQ e  */
#line 721 "limbaj.y"
           { if((yyvsp[-2].ast)->type==STRING||(yyvsp[-2].ast)->type==CHAR||(yyvsp[0].ast)->type==STRING||(yyvsp[0].ast)->type==CHAR)
   {fprintf(stderr,"Error at line : %d you cannot do string expressions\n",yylineno);exit(0);}
   (yyval.ast) = buildAST(strdup("=="), (yyvsp[-2].ast), (yyvsp[0].ast), OPERATOR); }
#line 2402 "limbaj.tab.c"
    break;

  case 107: /* e: e LEQ e  */
#line 724 "limbaj.y"
            { if((yyvsp[-2].ast)->type==STRING||(yyvsp[-2].ast)->type==CHAR||(yyvsp[0].ast)->type==STRING||(yyvsp[0].ast)->type==CHAR)
   {fprintf(stderr,"Error at line : %d you cannot do string expressions\n",yylineno);exit(0);}
   (yyval.ast) = buildAST(strdup("<="), (yyvsp[-2].ast), (yyvsp[0].ast), OPERATOR); }
#line 2410 "limbaj.tab.c"
    break;

  case 108: /* e: e AND e  */
#line 727 "limbaj.y"
            { if((yyvsp[-2].ast)->type==STRING||(yyvsp[-2].ast)->type==CHAR||(yyvsp[0].ast)->type==STRING||(yyvsp[0].ast)->type==CHAR)
   {fprintf(stderr,"Error at line : %d you cannot do string expressions\n",yylineno);exit(0);}
   (yyval.ast) = buildAST(strdup("||"), (yyvsp[-2].ast), (yyvsp[0].ast), OPERATOR); }
#line 2418 "limbaj.tab.c"
    break;

  case 109: /* e: e OR e  */
#line 730 "limbaj.y"
           { if((yyvsp[-2].ast)->type==STRING||(yyvsp[-2].ast)->type==CHAR||(yyvsp[0].ast)->type==STRING||(yyvsp[0].ast)->type==CHAR)
   {fprintf(stderr,"Error at line : %d you cannot do string expressions\n",yylineno);exit(0);}
   (yyval.ast) = buildAST(strdup("||"), (yyvsp[-2].ast), (yyvsp[0].ast), OPERATOR); }
#line 2426 "limbaj.tab.c"
    break;

  case 110: /* e: NOT e  */
#line 733 "limbaj.y"
          { if((yyvsp[0].ast)->type==STRING||(yyvsp[0].ast)->type==CHAR)
   {fprintf(stderr,"Error at line : %d you cannot do string expressions\n",yylineno);exit(0);}
   (yyval.ast) = buildAST(strdup("!"), (yyvsp[0].ast), nullptr, OPERATOR); }
#line 2434 "limbaj.tab.c"
    break;

  case 111: /* e: '(' e ')'  */
#line 736 "limbaj.y"
              { if((yyvsp[-1].ast)->type==STRING||(yyvsp[-1].ast)->type==CHAR)
   {fprintf(stderr,"Error at line : %d you cannot do string expressions\n",yylineno);exit(0);}
   (yyval.ast) = (yyvsp[-1].ast); }
#line 2442 "limbaj.tab.c"
    break;

  case 112: /* e: ID  */
#line 739 "limbaj.y"
       {
    if (!ids.existsVar((yyvsp[0].string))) {
        fprintf(stderr, "Error at line %d: Variable %s not declared\n", yylineno, (yyvsp[0].string));
        exit(0);
    }
    (yyval.ast) = buildAST((yyvsp[0].string), NULL, NULL, IDENTIFICATOR);
}
#line 2454 "limbaj.tab.c"
    break;

  case 113: /* e: NR  */
#line 746 "limbaj.y"
       { char nr[100]; bzero(&nr, 100); sprintf(nr, "%d", (yyvsp[0].val)); (yyval.ast) = buildAST(nr, NULL, NULL, NUMAR); }
#line 2460 "limbaj.tab.c"
    break;

  case 114: /* e: FLOATNR  */
#line 747 "limbaj.y"
            { char nr[100]; bzero(&nr, 100); sprintf(nr, "%f", (yyvsp[0].fval)); (yyval.ast) = buildAST(nr, NULL, NULL, NUMAR_FLOAT); }
#line 2466 "limbaj.tab.c"
    break;

  case 115: /* e: TRUE  */
#line 748 "limbaj.y"
         { char nr[4]; bzero(&nr, 4); sprintf(nr, "%d", 1);(yyval.ast) = buildAST(nr, NULL, NULL, BOOL); }
#line 2472 "limbaj.tab.c"
    break;

  case 116: /* e: FALSE  */
#line 749 "limbaj.y"
          { char nr[4]; bzero(&nr, 4); sprintf(nr, "%d", 0);(yyval.ast) = buildAST(nr, NULL, NULL, BOOL); }
#line 2478 "limbaj.tab.c"
    break;

  case 117: /* e: CHR  */
#line 750 "limbaj.y"
        {(yyval.ast) = buildAST(strdup((yyvsp[0].string)), nullptr, nullptr, CHAR);}
#line 2484 "limbaj.tab.c"
    break;

  case 118: /* e: STR  */
#line 751 "limbaj.y"
        {(yyval.ast) = buildAST(strdup((yyvsp[0].string)), nullptr, nullptr, STRING);}
#line 2490 "limbaj.tab.c"
    break;

  case 119: /* e: ID '.' ID  */
#line 752 "limbaj.y"
              {
    if (!ids.existsVar((yyvsp[0].string))|| (yyvsp[-2].string)!=ids.getDeclSpace((yyvsp[0].string)) ) {
        fprintf(stderr, "Error at line %d: Variable %s not declared\n", yylineno, (yyvsp[-2].string));
        exit(0);
    }
    (yyval.ast) = buildAST((yyvsp[0].string), NULL, NULL, IDENTIFICATOR);
}
#line 2502 "limbaj.tab.c"
    break;

  case 120: /* e: ID '(' call_list ')'  */
#line 759 "limbaj.y"
                         { if(ids.existsFct((yyvsp[-3].string))==0||"global"!=ids.getDeclSpaceFct((yyvsp[-3].string))){
          fprintf(stderr,"Error at line %d function %s not declared\n",yylineno,(yyvsp[-3].string)); exit(0);}
          else if(ids.paramsFct((yyvsp[-3].string),(yyvsp[-1].pparam)->paramCount,(yyvsp[-1].pparam)->params)==0){
          fprintf(stderr,"Error at line %d function %s does not have the right parameters\n",
          yylineno,(yyvsp[-3].string)); exit(0);}
          if(ids.getFctRetType((yyvsp[-3].string))=="int")
                            (yyval.ast) = buildAST(strdup("0"), NULL, NULL, NUMAR);
          if(ids.getFctRetType((yyvsp[-3].string))=="char")
                            (yyval.ast) = buildAST(strdup("0"), NULL, NULL, CHAR);
          if(ids.getFctRetType((yyvsp[-3].string))=="string")
                            (yyval.ast) = buildAST(strdup("0"), NULL, NULL, STRING);
          if(ids.getFctRetType((yyvsp[-3].string))=="float")
                            (yyval.ast) = buildAST(strdup("0"), NULL, NULL, NUMAR_FLOAT);
          if(ids.getFctRetType((yyvsp[-3].string))=="bool")
                            (yyval.ast) = buildAST(strdup("0"), NULL, NULL, BOOL);
               }
#line 2523 "limbaj.tab.c"
    break;

  case 121: /* e: ID '.' ID '(' call_list ')'  */
#line 775 "limbaj.y"
                                { if(ids.existsFct((yyvsp[-3].string))==0||(yyvsp[-5].string)!=ids.getDeclSpaceFct((yyvsp[-3].string))){
          fprintf(stderr,"Error at line %d function %s not declared\n",yylineno,(yyvsp[-5].string)); exit(0);}
          else if(ids.paramsFct((yyvsp[-3].string),(yyvsp[-1].pparam)->paramCount,(yyvsp[-1].pparam)->params)==0){
          fprintf(stderr,"Error at line %d function %s does not have the right parameters\n",
          yylineno,(yyvsp[-3].string)); exit(0);}
          if(ids.getFctRetType((yyvsp[-3].string))=="int")
                            (yyval.ast) = buildAST(strdup("0"), NULL, NULL, NUMAR);
          if(ids.getFctRetType((yyvsp[-3].string))=="char")
                            (yyval.ast) = buildAST(strdup("0"), NULL, NULL, CHAR);
          if(ids.getFctRetType((yyvsp[-3].string))=="string")
                            (yyval.ast) = buildAST(strdup("0"), NULL, NULL, STRING);
          if(ids.getFctRetType((yyvsp[-3].string))=="float")
                            (yyval.ast) = buildAST(strdup("0"), NULL, NULL, NUMAR_FLOAT);
          if(ids.getFctRetType((yyvsp[-3].string))=="bool")
                            (yyval.ast) = buildAST(strdup("0"), NULL, NULL, BOOL);
               }
#line 2544 "limbaj.tab.c"
    break;

  case 122: /* e: ID '(' ')'  */
#line 791 "limbaj.y"
               { {if(ids.existsFct((yyvsp[-2].string))==0||"global"!=ids.getDeclSpaceFct((yyvsp[-2].string))){
          fprintf(stderr,"Error at line %d function %s not declared\n",yylineno,(yyvsp[-2].string)); exit(0);}
          }
          if(ids.getFctRetType((yyvsp[-2].string))=="int")
                            (yyval.ast) = buildAST(strdup("0"), NULL, NULL, NUMAR);
          if(ids.getFctRetType((yyvsp[-2].string))=="char")
                            (yyval.ast) = buildAST(strdup("0"), NULL, NULL, CHAR);
          if(ids.getFctRetType((yyvsp[-2].string))=="string")
                            (yyval.ast) = buildAST(strdup("0"), NULL, NULL, STRING);
          if(ids.getFctRetType((yyvsp[-2].string))=="float")
                            (yyval.ast) = buildAST(strdup("0"), NULL, NULL, NUMAR_FLOAT);
          if(ids.getFctRetType((yyvsp[-2].string))=="bool")
                            (yyval.ast) = buildAST(strdup("0"), NULL, NULL, BOOL);
               }
#line 2563 "limbaj.tab.c"
    break;

  case 123: /* e: ID '.' ID '(' ')'  */
#line 805 "limbaj.y"
                      { {if(ids.existsFct((yyvsp[-2].string))==0||(yyvsp[-4].string)!=ids.getDeclSpaceFct((yyvsp[-2].string))){
          fprintf(stderr,"Error at line %d function %s not declared\n",yylineno,(yyvsp[-4].string)); exit(0);}
          }
          if(ids.getFctRetType((yyvsp[-2].string))=="int")
                            (yyval.ast) = buildAST(strdup("0"), NULL, NULL, NUMAR);
          if(ids.getFctRetType((yyvsp[-2].string))=="char")
                            (yyval.ast) = buildAST(strdup("0"), NULL, NULL, CHAR);
          if(ids.getFctRetType((yyvsp[-2].string))=="string")
                            (yyval.ast) = buildAST(strdup("0"), NULL, NULL, STRING);
          if(ids.getFctRetType((yyvsp[-2].string))=="float")
                            (yyval.ast) = buildAST(strdup("0"), NULL, NULL, NUMAR_FLOAT);
          if(ids.getFctRetType((yyvsp[-2].string))=="bool")
                            (yyval.ast) = buildAST(strdup("0"), NULL, NULL, BOOL);
               }
#line 2582 "limbaj.tab.c"
    break;

  case 124: /* e: ID '[' NR ']'  */
#line 819 "limbaj.y"
                  {
    if (!ids.existsVar((yyvsp[-3].string))) {
        fprintf(stderr, "Error at line %d: Variable %s not declared\n", yylineno, (yyvsp[-3].string));
        exit(0);
    }
    ind=(yyvsp[-1].val);
    (yyval.ast) = buildAST((yyvsp[-3].string), NULL, NULL, ELEM_OF_VECTOR);
}
#line 2595 "limbaj.tab.c"
    break;

  case 125: /* tipex: tipex '+' tipex  */
#line 830 "limbaj.y"
                        {
                                    if(strcmp((yyvsp[-2].string), (yyvsp[0].string)))
                                    {
                                        fprintf(stderr, "Error at line %d: different data types\n", yylineno);
                                        exit(0);
                                    }
                                    (yyval.string) = (yyvsp[-2].string);
                                 }
#line 2608 "limbaj.tab.c"
    break;

  case 126: /* tipex: tipex '-' tipex  */
#line 838 "limbaj.y"
                           {
                                    if(strcmp((yyvsp[-2].string), (yyvsp[0].string)))
                                    {
                                        fprintf(stderr, "Error at line %d: different data types\n", yylineno);
                                        exit(0);
                                    }
                                    (yyval.string) = (yyvsp[-2].string);
                                 }
#line 2621 "limbaj.tab.c"
    break;

  case 127: /* tipex: tipex '/' tipex  */
#line 846 "limbaj.y"
                           {
                                    if(strcmp((yyvsp[-2].string), (yyvsp[0].string)))
                                    {
                                        fprintf(stderr, "Error at line %d: different data types\n", yylineno);
                                        exit(0);
                                    }
                                    (yyval.string) = (yyvsp[-2].string);
                                 }
#line 2634 "limbaj.tab.c"
    break;

  case 128: /* tipex: tipex '*' tipex  */
#line 854 "limbaj.y"
                           {
                                    if(strcmp((yyvsp[-2].string), (yyvsp[0].string)))
                                    {
                                        fprintf(stderr, "Error at line %d: different data types\n", yylineno);
                                        exit(0);
                                    }
                                    (yyval.string) = (yyvsp[-2].string);
                                 }
#line 2647 "limbaj.tab.c"
    break;

  case 129: /* tipex: tipex '<' tipex  */
#line 862 "limbaj.y"
                           {
                                    if(strcmp((yyvsp[-2].string), (yyvsp[0].string)))
                                    {
                                        fprintf(stderr, "Error at line %d: different data types\n", yylineno);
                                        exit(0);
                                    }
                                    (yyval.string) = (yyvsp[-2].string);
                                 }
#line 2660 "limbaj.tab.c"
    break;

  case 130: /* tipex: tipex '>' tipex  */
#line 870 "limbaj.y"
                           {
                                    if(strcmp((yyvsp[-2].string), (yyvsp[0].string)))
                                    {
                                        fprintf(stderr, "Error at line %d: different data types\n", yylineno);
                                        exit(0);
                                    }
                                    (yyval.string) = (yyvsp[-2].string);
                                 }
#line 2673 "limbaj.tab.c"
    break;

  case 131: /* tipex: tipex GEQ tipex  */
#line 878 "limbaj.y"
                           {
                                    if(strcmp((yyvsp[-2].string), (yyvsp[0].string)))
                                    {
                                        fprintf(stderr, "Error at line %d: different data types\n", yylineno);
                                        exit(0);
                                    }
                                    (yyval.string) = (yyvsp[-2].string);
                                 }
#line 2686 "limbaj.tab.c"
    break;

  case 132: /* tipex: tipex EQ tipex  */
#line 886 "limbaj.y"
                          {
                                    if(strcmp((yyvsp[-2].string), (yyvsp[0].string)))
                                    {
                                        fprintf(stderr, "Error at line %d: different data types\n", yylineno);
                                        exit(0);
                                    }
                                    (yyval.string) = (yyvsp[-2].string);
                                 }
#line 2699 "limbaj.tab.c"
    break;

  case 133: /* tipex: tipex LEQ tipex  */
#line 894 "limbaj.y"
                           {
                                    if(strcmp((yyvsp[-2].string), (yyvsp[0].string)))
                                    {
                                        fprintf(stderr, "Error at line %d: different data types\n", yylineno);
                                        exit(0);
                                    }
                                    (yyval.string) = (yyvsp[-2].string);
                                 }
#line 2712 "limbaj.tab.c"
    break;

  case 134: /* tipex: tipex AND tipex  */
#line 902 "limbaj.y"
                           {
                                    if(strcmp((yyvsp[-2].string), (yyvsp[0].string)))
                                    {
                                        fprintf(stderr, "Error at line %d: different data types\n", yylineno);
                                        exit(0);
                                    }
                                    (yyval.string) = (yyvsp[-2].string);
                                 }
#line 2725 "limbaj.tab.c"
    break;

  case 135: /* tipex: tipex OR tipex  */
#line 910 "limbaj.y"
                          {
                                    if(strcmp((yyvsp[-2].string), (yyvsp[0].string)))
                                    {
                                        fprintf(stderr, "Error at line %d: different data types\n", yylineno);
                                        exit(0);
                                    }
                                    (yyval.string) = (yyvsp[-2].string);
                                 }
#line 2738 "limbaj.tab.c"
    break;

  case 136: /* tipex: NOT tipex  */
#line 918 "limbaj.y"
                     { (yyval.string) = (yyvsp[0].string); }
#line 2744 "limbaj.tab.c"
    break;

  case 137: /* tipex: '(' tipex ')'  */
#line 919 "limbaj.y"
                         { (yyval.string) = (yyvsp[-1].string); }
#line 2750 "limbaj.tab.c"
    break;

  case 138: /* tipex: ID  */
#line 920 "limbaj.y"
              { if(ids.existsVar((yyvsp[0].string)) == 0){
         fprintf(stderr,"Error at line %d variable %s not declared\n",yylineno,(yyvsp[0].string));
         exit(0);
         }
         (yyval.string) = strdup(ids.getIdType((yyvsp[0].string)).c_str());
         }
#line 2761 "limbaj.tab.c"
    break;

  case 139: /* tipex: NR  */
#line 926 "limbaj.y"
              {(yyval.string) = strdup("int");}
#line 2767 "limbaj.tab.c"
    break;

  case 140: /* tipex: TRUE  */
#line 927 "limbaj.y"
                {(yyval.string) = strdup("bool");}
#line 2773 "limbaj.tab.c"
    break;

  case 141: /* tipex: FALSE  */
#line 928 "limbaj.y"
                 {(yyval.string) = strdup("bool");}
#line 2779 "limbaj.tab.c"
    break;

  case 142: /* tipex: FLOATNR  */
#line 929 "limbaj.y"
                   {(yyval.string) = strdup("float");}
#line 2785 "limbaj.tab.c"
    break;

  case 143: /* tipex: STR  */
#line 930 "limbaj.y"
               {(yyval.string) = strdup("string");}
#line 2791 "limbaj.tab.c"
    break;

  case 144: /* tipex: CHR  */
#line 931 "limbaj.y"
               {(yyval.string) = strdup("char");}
#line 2797 "limbaj.tab.c"
    break;

  case 145: /* tipex: ID '[' NR ']'  */
#line 932 "limbaj.y"
                         { if(ids.existsVar((yyvsp[-3].string)) == 0){
         fprintf(stderr,"Error at line %d variable %s not declared\n",yylineno,(yyvsp[-3].string));
         exit(0);
         }
         (yyval.string) = strdup(ids.getIdType((yyvsp[-3].string)).c_str());
         }
#line 2808 "limbaj.tab.c"
    break;

  case 146: /* tipex: ID '(' call_list ')'  */
#line 938 "limbaj.y"
                                { if(ids.existsFct((yyvsp[-3].string))==0||"global"!=ids.getDeclSpaceFct((yyvsp[-3].string))){
          fprintf(stderr,"Error at line %d function %s not declared\n",yylineno,(yyvsp[-3].string)); exit(0);}
          else if(ids.paramsFct((yyvsp[-3].string),(yyvsp[-1].pparam)->paramCount,(yyvsp[-1].pparam)->params)==0){
          fprintf(stderr,"Error at line %d function %s does not have the right parameters\n",
          yylineno,(yyvsp[-3].string)); exit(0);}
          (yyval.string) = strdup(ids.getFctRetType((yyvsp[-3].string)).c_str()); 
          }
#line 2820 "limbaj.tab.c"
    break;

  case 147: /* tipex: ID '.' ID  */
#line 945 "limbaj.y"
                     {if(ids.existsVar((yyvsp[0].string))==0||(yyvsp[-2].string)!=ids.getDeclSpaceFct((yyvsp[0].string))){
          fprintf(stderr,"Error at line %d function %s not declared\n",yylineno,(yyvsp[-2].string)); exit(0);}
            (yyval.string) = strdup(ids.getFctRetType((yyvsp[0].string)).c_str());
         }
#line 2829 "limbaj.tab.c"
    break;

  case 148: /* tipex: ID '.' ID '(' call_list ')'  */
#line 949 "limbaj.y"
                                       { if(ids.existsFct((yyvsp[-3].string))==0||(yyvsp[-5].string)!=ids.getDeclSpaceFct((yyvsp[-3].string))){
          fprintf(stderr,"Error at line %d function %s not declared\n",yylineno,(yyvsp[-5].string)); exit(0);}
          else if(ids.paramsFct((yyvsp[-3].string),(yyvsp[-1].pparam)->paramCount,(yyvsp[-1].pparam)->params)==0){
          fprintf(stderr,"Error at line %d function %s does not have the right parameters\n",
          yylineno,(yyvsp[-5].string)); exit(0);}
            (yyval.string) = strdup(ids.getFctRetType((yyvsp[-3].string)).c_str());
         }
#line 2841 "limbaj.tab.c"
    break;


#line 2845 "limbaj.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 958 "limbaj.y"

void yyerror(const char * s){
printf("error: %s at line:%d\n",s,yylineno);
}

int main(int argc, char** argv){
     yyin=fopen(argv[1],"r");
     yyparse();
     ids.printVars();
    
}
