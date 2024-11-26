#pragma once
#include "SimpleSerialization.h"
#include <map>
#include <unordered_map>

#pragma region MAP

template<typename key, typename T>
void staticSerialize(Streamable& output, std::map<key, T>& var)
{
    size_t s = var.size();
    output.write(&s, sizeof(size_t)); //write size of list
    for(std::pair<key, T>& pair : var)
    {
        staticSerialize(output, pair.first); //write key
        staticSerialize(output, pair.second); //write data
    }
}

template<typename key, typename T>
void staticDeserialize(Streamable& input, std::map<key, T>& var)
{
    size_t s = 0;
    input.read(&s, sizeof(size_t)); //read size of list
    var.clear();
    for(size_t i=0; i<s; i++)
    {
        std::pair<key, T> pair; //need default constructor
        staticDeserialize(input, pair.first);
        staticDeserialize(input, pair.second);
        var.insert(pair);
    }
}

template<typename key, typename T>
void staticSerialize(Streamable& output, std::multimap<key, T>& var)
{
    size_t s = var.size();
    output.write(&s, sizeof(size_t)); //write size of list
    for(std::pair<key, T>& pair : var)
    {
        staticSerialize(output, pair.first); //write key
        staticSerialize(output, pair.second); //write data
    }
}

template<typename key, typename T>
void staticDeserialize(Streamable& input, std::multimap<key, T>& var)
{
    size_t s = 0;
    input.read(&s, sizeof(size_t)); //read size of list
    var.clear();
    for(size_t i=0; i<s; i++)
    {
        std::pair<key, T> pair; //need default constructor
        staticDeserialize(input, pair.first);
        staticDeserialize(input, pair.second);
        var.insert(pair);
    }
}

#pragma endregion

#pragma region UNORDERED_MAP

template<typename key, typename T>
void staticSerialize(Streamable& output, std::unordered_map<key, T>& var)
{
    size_t s = var.size();
    output.write(&s, sizeof(size_t)); //write size of list
    for(std::pair<key, T>& pair : var)
    {
        staticSerialize(output, pair.first); //write key
        staticSerialize(output, pair.second); //write data
    }
}

template<typename key, typename T>
void staticDeserialize(Streamable& input, std::unordered_map<key, T>& var)
{
    size_t s = 0;
    input.read(&s, sizeof(size_t)); //read size of list
    var = std::unordered_map<key, T>();
    for(size_t i=0; i<s; i++)
    {
        std::pair<key, T> pair; //need default constructor
        staticDeserialize(input, pair.first);
        staticDeserialize(input, pair.second);
        var.insert(pair);
    }
}

template<typename key, typename T>
void staticSerialize(Streamable& output, std::unordered_multimap<key, T>& var)
{
    size_t s = var.size();
    output.write(&s, sizeof(size_t)); //write size of list
    for(std::pair<key, T>& pair : var)
    {
        staticSerialize(output, pair.first); //write key
        staticSerialize(output, pair.second); //write data
    }
}

template<typename key, typename T>
void staticDeserialize(Streamable& input, std::unordered_multimap<key, T>& var)
{
    size_t s = 0;
    input.read(&s, sizeof(size_t)); //read size of list
    var = std::unordered_map<key, T>();
    for(size_t i=0; i<s; i++)
    {
        std::pair<key, T> pair; //need default constructor
        staticDeserialize(input, pair.first);
        staticDeserialize(input, pair.second);
        var.insert(pair);
    }
}

#pragma endregion