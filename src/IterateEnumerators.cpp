#pragma once
#include <iostream>
#include <boost/describe/enum.hpp>
#include <boost/describe/enumerators.hpp>
#include <boost/mp11/algorithm.hpp>

enum E
{
    v1 = 11,
    v2,
    v3 = 5
};

BOOST_DESCRIBE_ENUM(E, v1, v2, v3)

int main()
{
    boost::mp11::mp_for_each<boost::describe::describe_enumerators<E>>([](auto D)
    {
       std::cout << D.name << " " << D.value << std::endl; 
    });
}