#include "SimpleSerialization.h"
#include <any>
#include <functional>
#include <unordered_map>

class SerializeAnyFunctionMapper
{
public:
    static void mapFunction(std::string className, std::function<void(Streamable&, std::any&)> serializeFunction, std::function<void(Streamable&, std::any&)> deserializeFunction);
    static std::pair<std::function<void(Streamable&, std::any&)>, std::function<void(Streamable&, std::any&)>> getFunctions(std::string className);

private:
    static SerializeAnyFunctionMapper& getSingleton();
    SerializeAnyFunctionMapper() {}
    ~SerializeAnyFunctionMapper() {}
    std::unordered_map<std::string, std::pair<std::function<void(Streamable&, std::any&)>, std::function<void(Streamable&, std::any&)>>> functionMappings;
};

inline SerializeAnyFunctionMapper& SerializeAnyFunctionMapper::getSingleton()
{
    static SerializeAnyFunctionMapper singleton;
    return singleton;
}

inline void SerializeAnyFunctionMapper::mapFunction(std::string className, std::function<void(Streamable&, std::any&)> serializeFunction, std::function<void(Streamable&, std::any&)> deserializeFunction)
{
    SerializeAnyFunctionMapper& singleton = getSingleton();
    singleton.functionMappings[className] = {serializeFunction, deserializeFunction};
}

inline std::pair<std::function<void(Streamable&, std::any&)>, std::function<void(Streamable&, std::any&)>> SerializeAnyFunctionMapper::getFunctions(std::string className)
{
    SerializeAnyFunctionMapper& singleton = getSingleton();
    return singleton.functionMappings[className];
}


void staticSerialize(Streamable& output, std::any& var)
{
    //attempt to find a function for it in the mapper
    auto mappings = SerializeAnyFunctionMapper::getFunctions(var.type().name());
    if(mappings.first != nullptr)
        mappings.first(output, var);
    else
        throw nullptr; //TODO: THROW PROPER ERROR
}

void staticDeserialize(Streamable& input, std::any& var)
{
    //attempt to find a function for it in the mapper
    auto mappings = SerializeAnyFunctionMapper::getFunctions(var.type().name());
    if(mappings.first != nullptr)
        mappings.second(input, var);
    else
        throw nullptr; //TODO: THROW PROPER ERROR
}