#include <boost/describe.hpp>
#include <boost/mp11.hpp>
#include <iostream>

template <class E,
          std::enable_if_t<std::is_enum_v<E>, int>  = 0> //enable the function only for enums
const char* EnumTostring(E e)
{
    auto r = "unnamed";
    boost::mp11::mp_for_each<boost::describe::describe_enumerators<E>>([&](auto D)
    {
        if (e == D.value)
        {
            r = D.name;
        }
    });

    return r;
}

enum E
{
    v1 = 3,
    v2,
    v3 = 11
};

BOOST_DESCRIBE_ENUM(E, v1, v2, v3)

int main()
{
    std::cout << "E(" << v1 << "): " << EnumTostring(v1) << std::endl;
}
