#pragma once
#include "SimpleSerialization.h"
#include <array>

template<typename T, size_t S>
void staticSerialize(Streamable& output, std::array<T, S>& var)
{
    size_t writeSize = S;
    output.write(&writeSize, sizeof(size_t)); //write size of list
    for(size_t i=0; i<writeSize; i++)
    {
        staticSerialize(output, var[i]);
    }
}

template<typename T, size_t S>
void staticDeserialize(Streamable& input, std::array<T, S>& var)
{
    size_t readSize = 0;
    input.read(&readSize, sizeof(size_t)); //read size of list
    //S must readSize. otherwise throw an error
    if(S != readSize)
        throw S; //TODO: FIX THIS. Should throw proper exceptions
    
    for(size_t i=0; i<readSize; i++)
    {
        staticDeserialize(input, var[i]); //need default constructor
    }
}