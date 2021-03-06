#ifndef __VARIABLE_H__
#define __VARIABLE_H__

#include <string>

#include "libc_func.h"

#ifdef _X64
typedef uint64_t PTR_TYPE;
#else
typedef uint32_t PTR_TYPE;
#endif

namespace Mini
{

enum class VariableType
{
    VT_bool,
    VT_char,
    VT_short,
    VT_unsigned_short,
    VT_int,
    VT_unsigned_int,
    VT_char_array,
    VT_offset
};

class Variable
{
public:
    Variable(const VariableType, const PTR_TYPE&);
    ~Variable();
    
    PTR_TYPE GetData() const;
    VariableType GetVarType() const;
    std::string ToString() const;
private:
    PTR_TYPE _data;
    VariableType _var_type;
};

}

#endif // __VARIABLE_H__
