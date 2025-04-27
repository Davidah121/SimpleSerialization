#include "TypeInfo.h"

#ifndef _MSC_VER
#include <cstdlib>
#include <cxxabi.h>
#endif

std::string demangleClassName(std::string name)
{
    #ifndef _MSC_VER
    std::string finalName = "";
    int status = 0;
    char* realName = abi::__cxa_demangle(name.c_str(), 0, 0, &status);
    finalName = realName;
    std::free(realName);
    return finalName;
    #else
    return name;
    #endif
}