#pragma once
#include "SimpleSerialization.h"
#include <vector>

template<typename T>
void staticSerialize(Streamable& output, std::vector<T>& var)
{
    size_t s = var.size();
    output.write(&s, sizeof(size_t)); //write size of list
    for(size_t i=0; i<var.size(); i++)
    {
        staticSerialize(output, var[i]);
    }
}

template<typename T>
void staticDeserialize(Streamable& input, std::vector<T>& var)
{
    size_t s = 0;
    input.read(&s, sizeof(size_t)); //read size of list
    var = std::vector<T>(s);
    for(size_t i=0; i<var.size(); i++)
    {
        staticDeserialize(input, var[i]); //need default constructor
    }
}