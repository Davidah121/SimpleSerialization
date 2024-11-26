#pragma once
#include "SimpleSerialization.h"
#include <tuple>

template<size_t I = 0, typename... Types>
typename std::enable_if<I == sizeof...(Types), void>::type
staticSerialize(Streamable& output, std::tuple<Types...>& var)
{
}

template<size_t I = 0, typename... Types>
typename std::enable_if<I < sizeof...(Types), void>::type
staticSerialize(Streamable& output, std::tuple<Types...>& var)
{
    staticSerialize(output, std::get<I>(var));
    staticSerialize<I+1, Types...>(output, var);
}

template<size_t I = 0, typename... Types>
typename std::enable_if<I == sizeof...(Types), void>::type
staticDeserialize(Streamable& output, std::tuple<Types...>& var)
{
}

template<size_t I = 0, typename... Types>
typename std::enable_if<I < sizeof...(Types), void>::type
staticDeserialize(Streamable& output, std::tuple<Types...>& var)
{
    staticDeserialize(output, std::get<I>(var));
    staticDeserialize<I+1, Types...>(output, var);
}