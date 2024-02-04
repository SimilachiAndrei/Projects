#include "IdList.h"
#include <cstring>
using namespace std;

void IdList::updateInArr(int index, string id, string type, int ival, bool bval, char chrval, float fval)
{
    for (ArrInfo &v : arrays)
    {
        if (id == v.name)
        {
            if (type == "int")
            {
                v.arrVal.ival[index] = ival;
            }
            else if (type == "char")
            {
                v.arrVal.chrval[index] = chrval;
            }
            else if (type == "float")
            {
                v.arrVal.fval[index] = fval;
            }
            else if (type == "bool")
            {
                v.arrVal.bval[index] = bval;
            }
        }
    }
}

void IdList::addVar(bool isConst, const char *type, const char *name, int ival, float fval, bool bval,
                    const char *strval, char chr, const char *declIn)
{
    IdInfo var;
    var.isConst = isConst;
    var.type = string(type);
    var.name = string(name);
    var.val.ival = ival;
    var.val.fval = fval;
    var.val.bval = bval;
    var.val.strval = string(strval);
    var.val.chrval = chr;
    var.declIn = string(declIn);
    vars.push_back(var);
}
void IdList::updateVar(string id, string type, int ival, bool bval, char chrval, string strval, float fval)
{
    for (IdInfo &v : vars)
    {
        if (id == v.name)
        {
            if (type == "int")
            {
                v.val.ival = ival;
            }
            else if (type == "char")
            {
                v.val.chrval = chrval;
            }
            else if (type == "float")
            {
                v.val.fval = fval;
            }
            else if (type == "bool")
            {
                v.val.bval = bval;
            }
            else if (type == "string")
            {
                v.val.strval = strval;
            }
        }
    }
}

int IdList::isConst(string name)
{
    for (IdInfo &v : vars)
    {
        if (v.name == name)
        {
            if (v.isConst == true)
                return 1;
            else
                return 0;
        }
    }
    return -1;
}

bool IdList::existsVar(const char *var)
{
    string strvar = string(var);
    for (const IdInfo &v : vars)
    {
        if (strvar == v.name)
        {
            return true;
        }
    }
    for (const ArrInfo &v : arrays)
    {
        if (strvar == v.name)
        {
            return true;
        }
    }
    for (const ClsInfo &v : classes)
    {
        if (strvar == v.name)
        {
            return true;
        }
    }
    return false;
}

bool IdList::existsArr(const char *s)
{
    for (const ArrInfo &v : arrays)
    {
        if (s == v.name)
        {
            return true;
        }
    }
    return false;
}

void IdList::addArray(int size, const char *type, const char *name, int ival, float fval, bool bval,
                      const char *strval, const char *declIn)
{
    ArrInfo var;
    var.size = size;
    var.type = string(type);
    var.name = string(name);
    var.arrVal.chrval = string(strval);

    // Initialize arrays with zeros
    if (strcmp(type, "int") == 0)
    {
        for (int i = 0; i < size; ++i)
        {
            var.arrVal.ival[i] = ival;
        }
    }
    else if (strcmp(type, "bool") == 0)
    {
        for (int i = 0; i < size; ++i)
        {
            var.arrVal.bval[i] = bval;
        }
    }
    else if (strcmp(type, "float") == 0)
    {
        for (int i = 0; i < size; ++i)
        {
            var.arrVal.fval[i] = fval;
        }
    }

    var.declIn = string(declIn);
    arrays.push_back(var);
}

void IdList::printVars()
{
    FILE *fd = fopen("symboltable.txt", "wb");
    if (fd == nullptr)
    {
        fprintf(stderr, "Error: Unable to open symboltable.txt for writing\n");
        exit(EXIT_FAILURE);
    }

    char *symboltable = (char *)malloc(5000 * sizeof(char));
    if (symboltable == nullptr)
    {
        fclose(fd);
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    bzero(symboltable, 5000);
    for (const IdInfo &v : vars)
    {
        char buffer[512]; // Adjust the size as needed
        snprintf(buffer, sizeof(buffer), "const: %d name: %s type: %s value: ", v.isConst, v.name.c_str(), v.type.c_str());

        if (v.type == "int")
            snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), "%d", v.val.ival);
        else if (v.type == "bool")
            snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), "%d", v.val.bval);
        else if (v.type == "string")
            snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), "%s", v.val.strval.c_str());
        else if (v.type == "char")
            snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), "%c", v.val.chrval);
        else if (v.type == "float")
            snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), "%f", v.val.fval);

        snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), " place of declaration: %s\n", v.declIn.c_str());
        strcat(symboltable, buffer);
    }

    for (const ArrInfo &v : arrays)
    {
        char buffer[512]; // Adjust the size as needed
        snprintf(buffer, sizeof(buffer), "Array name: %s type: %s size: %d values: ", v.name.c_str(), v.type.c_str(), v.size);

        if (v.type == "int")
            for (int i = 0; i < v.size; i++)
                snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), "%d ", v.arrVal.ival[i]);
        else if (v.type == "bool")
            for (int i = 0; i < v.size; i++)
                snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), "%d ", v.arrVal.bval[i]);
        else if (v.type == "float")
            for (int i = 0; i < v.size; i++)
                snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), "%f ", v.arrVal.fval[i]);

        snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), " place of declaration: %s\n", v.declIn.c_str());
        strcat(symboltable, buffer);
    }

    // Print Functions
    strcat(symboltable, "Functions:\n");
    for (const FctInfo &fct : functions)
    {
        char buffer[512]; // Adjust the size as needed
        snprintf(buffer, sizeof(buffer), "Function type: %s name: %s place of declaration: %s\n", fct.type.c_str(), fct.name.c_str(), fct.declIn.c_str());

        // Print Parameters
        for (int i = 0; i < fct.paramCount; ++i)
        {
            snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), "Parameter %d type: %s\n", i + 1, fct.paramType[i].c_str());
        }

        strcat(symboltable, buffer);
    }

    fwrite(symboltable, 1, strlen(symboltable), fd);
    free(symboltable);
    fclose(fd);
}

IdList::~IdList()
{
    vars.clear();
    arrays.clear();
    functions.clear();
}

bool IdList::existsFct(const char *s)
{
    for (const FctInfo &fct : functions)
    {
        if (fct.name == s)
        {
            return true;
        }
    }
    return false;
}

bool IdList::paramsFct(const char *functionName, int paramCount, string params[])
{
    for (const FctInfo &fct : functions)
    {
        if (fct.name == functionName)
        {
            if (fct.paramCount != paramCount)
            {
                return false;
            }

            for (int i = 0; i < paramCount; i++)
            {
                if (params[i] != fct.paramType[i])
                {
                    return false;
                }
            }

            return true;
        }
    }

    return false;
}

void IdList::addFunc(const char *type, const char *name, int ival, string param[], const char *declIn)
{
    FctInfo newFct;
    newFct.type = type;
    newFct.name = name;
    newFct.declIn = declIn;
    newFct.paramCount = ival;
    // Assuming ival is the number of parameters
    for (int i = 0; i < ival; ++i)
    {
        newFct.paramType[i] = param[i];
    }

    functions.push_back(newFct);
}

void IdList::addClass(const char *type)
{
    ClsInfo cls{string(type)};
    classes.push_back(cls);
}

int IdList::getValueByIDi(string id)
{
    for (const IdInfo &v : vars)
    {
        if (id == v.name)
        {
            return v.val.ival;
        }
    }
    return 0;
}

float IdList::getValueByIDf(string id)
{
    for (const IdInfo &v : vars)
    {
        if (id == v.name)
        {
            return v.val.fval;
        }
    }
    return 0;
}
int IdList::getValueByIDiV(string id, int index)
{
    for (const ArrInfo &v : arrays)
    {
        if (id == v.name)
        {
            return v.arrVal.ival[index];
        }
    }
    return 0;
}
float IdList::getValueByIDfV(string id, int index)
{
    for (const ArrInfo &v : arrays)
    {
        if (id == v.name)
        {
            return v.arrVal.fval[index];
        }
    }
    return 0;
}
string IdList::getIdType(string id)
{
    for (const IdInfo &v : vars)
    {
        if (id == v.name)
        {
            return v.type;
        }
    }
    for (const ArrInfo &v : arrays)
    {
        if (id == v.name)
        {
            return v.type;
        }
    }
    return "";
}

string IdList::getDeclSpace(string id)
{
    for (const IdInfo &v : vars)
    {
        if (id == v.name)
        {
            return v.declIn;
        }
    }
    for (const ArrInfo &v : arrays)
    {
        if (id == v.name)
        {
            return v.declIn;
        }
    }
    return "";
}
string IdList::getDeclSpaceFct(string id)
{
    for (const FctInfo &v : functions)
    {
        if (id == v.name)
        {
            return v.declIn;
        }
    }
    return "";
}

string IdList::getFctRetType(string id)
{
    for (const FctInfo &v : functions)
    {
        if (id == v.name)
        {
            return v.type;
        }
    }
    return "";
}

bool IdList::getValueByIDbool(string id)
{
    for (const IdInfo &v : vars)
    {
        if (id == v.name)
        {
            return v.val.bval;
        }
    }
    return 0;
}