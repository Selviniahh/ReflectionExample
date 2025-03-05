#include <iostream>
#include <boost/type_index.hpp>
#include <boost/describe.hpp>

//Primary template for classes/structs
template <class T,
          class Bd = boost::describe::describe_bases<T, boost::describe::mod_any_access>, //Base description
          class Md = boost::describe::describe_members<T, boost::describe::mod_any_access>, //Member description
          std::enable_if_t<!std::is_enum_v<T>, int>  = 0 //check if T is not enum
>
std::ostream& operator<<(std::ostream& os, const T& t)
{
    //Bases: 
    boost::mp11::mp_for_each<Bd>([&]<typename T0>(T0)
    {
        using BTN = typename T0::type; //BT stands for Base Type Name
        os << "Parent: " << boost::typeindex::type_id<BTN>().pretty_name();
        os << (BTN const&)t << std::endl;

        //Just for easy to debug remove here later
        auto sa = boost::typeindex::type_id<BTN>().pretty_name(); //X
        auto sec = (BTN const&)t; //m1
    });

    //Members:
    boost::mp11::mp_for_each<Md>([&](auto D)
    {
        using member_type = decltype(t.*D.pointer);
        auto actualTypeName = boost::typeindex::type_id<member_type>().pretty_name();
        auto TypeName = D.name;
        auto Value = t.*D.pointer;
        os << "." << D.name << " = " << t.*D.pointer << std::endl; //t.*D.pointer will invoke bottom specialization if D is enum

        //Check if current member is enum.
        if (std::is_enum_v<member_type>)
        {
            os << "Yesss";
        }
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


enum E
{
    v1 = 11,
    v2,
    v3 = 5
};

BOOST_DESCRIBE_ENUM(E, v1, v2, v3)


class Z : public X, private Y
{
    int m1 = 3;
    int m2 = 4;
    E ExEnum{};

    BOOST_DESCRIBE_CLASS(Z, (X, Y), (), (), (m1,m2,ExEnum))
};

int main()
{
    std::cout << Z() << std::endl; //Print the class names, values, parent name and values.
}
