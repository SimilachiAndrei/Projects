#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string>

using namespace std;

enum nodetype
{
    OPERATOR = 1,
    IDENTIFICATOR = 2,
    NUMAR = 3,
    NUMAR_FLOAT = 4,
    STRING_OR_CHAR = 5,
    BOOL = 6,
    ELEM_OF_VECTOR = 7,
    OTHERS = 0,
    STRING=8,
    CHAR=9
};

struct value
{
    int ival;
    float fval;
    bool bval;
    string strval;
    char chrval;
};

struct IdInfo
{
    bool isConst;
    string type;
    string name;
    value val;
    string declIn;
};

struct arrValue
{
    int ival[100];
    float fval[100];
    bool bval[100];
    string chrval;
};

struct ArrInfo
{
    int size;
    string type;
    string name;
    arrValue arrVal;
    string declIn;
};

struct FctInfo
{
    string type;
    string name;
    int paramCount;
    string paramType[50];
    string declIn;
};

struct ClsInfo
{
    string name;
};

class IdList
{
    vector<IdInfo> vars;
    vector<ArrInfo> arrays;
    vector<FctInfo> functions;
    vector<ClsInfo> classes;
    // maybe add a vector for class and to update the values of arrays and vars

public:
    bool existsVar(const char *s);
    bool existsFct(const char *s);
    bool existsArr(const char *s);
    bool paramsFct(const char *s, int count, string params[]);
    void addVar(bool isConst, const char *type, const char *name, int ival, float fval, bool bval,
                const char *strval, char chr, const char *declIn);
    void addArray(int size, const char *type, const char *name, int ival, float fval, bool bval,
                  const char *strval, const char *declIn);
    void addClass(const char *s);
    void addFunc(const char *type, const char *name, int ival, string param[], const char *declIn);
    int getValueByIDi(string id);
    float getValueByIDf(string id);
    int getValueByIDiV(string id, int index);
    float getValueByIDfV(string id, int index);
    bool getValueByIDbool(string id);
    string getIdType(string id);
    string getDeclSpace(string id);
    string getDeclSpaceFct(string id);
    string getFctRetType(string id);
    int isConst(string name);
    void updateVar(string id, string type, int ival, bool bval, char chrval,string strvar ,float fval);
    void updateInArr(int index, string id, string type, int ival, bool bval, char chrval, float fval);
    void printVars();
    ~IdList();
};

struct AST
{
    std::string name;
    struct AST *left;
    struct AST *right;
    enum nodetype type;
};