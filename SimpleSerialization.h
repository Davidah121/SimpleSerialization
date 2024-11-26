#pragma once
#include "PreprocessorTricks.h"
#include <bitset>
#include <typeinfo>
#include <string>
#include <unordered_map>
#include "Streamable.h"

class TypeInfoHash;
struct TypeInfo;
struct SerializedVariable;
class SerializedObject;

template<typename T, typename... Args>
void staticSerialize(Streamable& output, T& var1, Args&... var2);
template<typename T>
void staticSerialize(Streamable& output, T& var);
void staticSerialize(Streamable& output);

template<typename T, typename... Args>
void staticDeserialize(Streamable& input, T& var1, Args&... var2);
template<typename T>
void staticDeserialize(Streamable& input, T& var1);
void staticDeserialize(Streamable& input);

std::string demangleClassName(std::string name);

enum OPTIONS
{
    IS_INTEGER,
    IS_FLOAT,
    IS_SIGNED,
    IS_TRIVIAL,
    IS_POINTER,
    IS_SERIALIZABLE,
    IS_VOID,
    IS_TEMPLATE
};

struct TypeInfo final
{
    std::string name;
    uint32_t size;
    uint64_t hash;
    std::bitset<8> attributes;

    bool operator==(const TypeInfo& t) const
    {
        return hash == t.hash;
    }

    std::string getName()
    {
        #ifdef _unix_
            return demangleClassName((std::string)name);
        #else
            return (std::string)name;
        #endif
    }
    
    template <typename T>
    static TypeInfo get();
};

struct SerializedVariable final
{
    void* data;
    TypeInfo type;
};

class SerializedObject
{
public:
    virtual const TypeInfo getClass() const = 0;
    virtual void serialize(Streamable& output){} //for overriding
    virtual void deserialize(Streamable& input){} //for overriding
    virtual std::unordered_map<std::string, SerializedVariable> getSerializedVariables() { return {};}
};

#ifndef SERIALIZE
    #define SERIALIZE(var) {&var, TypeInfo::get<decltype(var)>()}
	#define SERIALIZE_MAP(var) {#var, SERIALIZE(var)}

    #define SERIALIZE_FUNCTION_DEFINITIONS()\
        virtual const TypeInfo getClass() const;\
        virtual void serialize(Streamable& output);\
        virtual void deserialize(Streamable& input);\
        virtual std::unordered_map<std::string, SerializedVariable> getSerializedVariables();

    #define SERIALIZE_CLASS_GETCLASS(T)\
        inline const TypeInfo T::getClass() const {return TypeInfo::get<T>();}

    #define SERIALIZE_CLASS_SERIALIZE_VARIABLES(T, ...)\
        inline std::unordered_map<std::string, SerializedVariable> T::getSerializedVariables()\
        { return {FOR_EACH_LIST(SERIALIZE_MAP, __VA_ARGS__)}; }

    #define SERIALIZE_CLASS_SERIALIZE_FUNCTIONS(T, ...)\
        inline void T::serialize(Streamable& output)\
        { staticSerialize(output, __VA_ARGS__); }\
        inline void T::deserialize(Streamable& input)\
        { staticDeserialize(input, __VA_ARGS__); }
    
    #define SERIALIZE_CLASS(T, ...)\
        SERIALIZE_CLASS_GETCLASS(T)\
        SERIALIZE_CLASS_SERIALIZE_FUNCTIONS(T, __VA_ARGS__)\
        SERIALIZE_CLASS_SERIALIZE_VARIABLES(T, __VA_ARGS__)
#endif


template<typename T>
inline TypeInfo TypeInfo::get()
{
    TypeInfo info;
    info.name = typeid(T).name();
    #if(T == void)
        info.size = 0;
    #else
        info.size = sizeof(T);
    #endif
    info.hash = typeid(T).hash_code();
    info.attributes[IS_INTEGER] = std::is_integral<T>();
    info.attributes[IS_FLOAT] = std::is_floating_point<T>();
    info.attributes[IS_SIGNED] = std::is_signed<T>();
    info.attributes[IS_TRIVIAL] = std::is_trivially_copyable<T>();
    info.attributes[IS_POINTER] = std::is_pointer<T>();
    info.attributes[IS_SERIALIZABLE] = std::is_base_of<SerializedObject, T>();
    info.attributes[IS_VOID] = std::is_void<T>();
    info.attributes[IS_TEMPLATE] = info.name.find('<') != SIZE_MAX;
    return info;
}

void staticSerialize(Streamable& output)
{
    //base case. do nothing
}

template<typename T, typename... Args>
void staticSerialize(Streamable& output, T& var1, Args&... var2)
{
    //print something
    staticSerialize(output, var1);
    staticSerialize(output, var2...);
}

template<typename T>
void staticSerialize(Streamable& output, T& var)
{
    //special clause for keeping things a bit simpler
    TypeInfo t = TypeInfo::get<T>();
    if(t.attributes[IS_SERIALIZABLE])
    {
        ((SerializedObject&)var).serialize(output);
    }
    else
    {
        output.write(&var, sizeof(T));
    }
}

void staticDeserialize(Streamable& input)
{
    //nothing. base case
}

template<typename T, typename... Args>
void staticDeserialize(Streamable& input, T& var1, Args&... var2)
{
    //print something
    staticDeserialize(input, var1);
    staticDeserialize(input, var2...);
}
template<typename T>
void staticDeserialize(Streamable& input, T& var)
{
    //special clause for keeping things a bit simpler
    TypeInfo t = TypeInfo::get<T>();
    if(t.attributes[IS_SERIALIZABLE])
    {
        ((SerializedObject&)var).deserialize(input);
    }
    else
    {
        input.read(&var, sizeof(T));
    }
}

inline std::string demangleClassName(std::string name)
{
    return name;
}