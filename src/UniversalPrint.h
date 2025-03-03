#include <boost/describe.hpp>
#include <boost/mp11.hpp>
#include <boost/type_index.hpp>
#include <ostream>
#include <iostream>

using namespace boost::describe;

template <class T,
          class Bd = describe_bases<T, mod_any_access>, //base description
          class Md = describe_members<T, mod_any_access>> //member description
std::ostream& operator<<(std::ostream& os, const T& t)
{
    //print parent's described members
    boost::mp11::mp_for_each<Bd>([&]<typename T0>(T0)
    {
        using B = typename T0::type;
        os << "Parent: " << boost::typeindex::type_id<B>().pretty_name();
        os << (const B&)t << std::endl; //const is necessary for private base classes
    });

    //Print own member variables
    boost::mp11::mp_for_each<Md>([&](auto D)
    {
        using member_type = decltype(t.*D.pointer);
        auto actualTypeNmae = boost::typeindex::type_id<member_type>().pretty_name();
        os << "." << D.name << " = " << t.*D.pointer << std::endl;
    });

    return os;
}

struct X
{
    int m1 = 1;
};

BOOST_DESCRIBE_STRUCT(X, (), (m1))

struct Y
{
    int m2 = 2;
};

BOOST_DESCRIBE_STRUCT(Y, (), (m2))

class Z : public X, private Y
{
    int m1 = 3;
    int m2 = 4;

    BOOST_DESCRIBE_CLASS(Z, (X, Y), (), (), (m1, m2))
};
