#pragma once
#include "SimpleSerialization.h"
#include <string>

template<typename T>
void staticSerialize(Streamable& output, std::basic_string<T>& var)
{
    size_t s = var.size()+1; //include null character
    output.write(&s, sizeof(size_t));
    for(T& c : var)
    {
        staticSerialize(output, c); //must be trivial but that doesn't mean simple
    }
}

template<typename T>
void staticDeserialize(Streamable& input, std::basic_string<T>& var)
{
    size_t s;
    input.read(&s, sizeof(size_t)); //read size
    var.reserve(s); //reserve data
    for(size_t i=0; i<s; i++)
    {
        staticDeserialize(input, var[i]); //must be trivial but that doesn't mean simple
    }
}

void staticSerialize(Streamable& output, std::string& var)
{
    size_t s = var.size()+1; //include null character
    output.write(&s, sizeof(size_t));
    output.write(var.c_str(), s*sizeof(char)); //primitive type. Can write directly
}

void staticDeserialize(Streamable& input, std::string& var)
{
    size_t s;
    input.read(&s, sizeof(size_t)); //read size
    var.reserve(s*sizeof(char)); //reserve data
    input.read((void*)var.data(), s*sizeof(char)); //read data into string
}

void staticSerialize(Streamable& output, std::wstring& var)
{
    size_t s = var.size()+1; //include null character
    output.write(&s, sizeof(size_t));
    output.write(var.c_str(), s*sizeof(wchar_t)); //primitive type. Can write directly
}

void staticDeserialize(Streamable& input, std::wstring& var)
{
    size_t s;
    input.read(&s, sizeof(size_t)); //read size
    var.reserve(s*sizeof(wchar_t)); //reserve data
    input.read((void*)var.data(), s*sizeof(wchar_t)); //read data into string
}