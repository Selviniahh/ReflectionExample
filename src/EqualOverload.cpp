#include <variant>
#include <boost/describe.hpp>
#include <boost/mp11.hpp>
#include <vector>
#include <boost/type_index.hpp>
#include <iostream>

//NOTE: Purpose of this is to check if compared object's bases and all member variables are same
using namespace boost::describe;

namespace app
{
    template <class T,
              class Bd = describe_bases<T, mod_any_access>,
              class Md = describe_members<T, mod_any_access>>
    bool operator==(T const& t1, T const& t2)
    {
        static_assert(!std::is_enum_v<T>, "Error: This operator== cannot be used with enum types.");
        bool r = true;

        boost::mp11::mp_for_each<Bd>([&]<typename T0>(T0)
        {
            using B = typename T0::type;
            r = r && (B const&)t1 == (B const&)t2;

            std::cout << boost::typeindex::type_id<B>().pretty_name() << std::endl; //X
            std::cout << (B const&)t1 << std::endl; //m1
            std::cout << (B const&)t2 << std::endl; //m1
        });

        boost::mp11::mp_for_each<Md>([&](auto D)
        {
            auto fir = t1.*D.pointer;
            auto sec = t2.*D.pointer;
            using member_type = decltype(t1.*D.pointer);
            auto actualTypeName = boost::typeindex::type_id<member_type>().pretty_name();

            r = r && t1.*D.pointer == t2.*D.pointer;
        });

        return r;
    }

    struct A
    {
        int x = 1;
    };

    BOOST_DESCRIBE_STRUCT(A, (), (x))

    struct B
    {
        int y = 2;
    };

    BOOST_DESCRIBE_STRUCT(B, (), (y))

    struct C
    {
        std::vector<std::variant<A, B>> v;
    };

    BOOST_DESCRIBE_STRUCT(C, (), (v))
} // namespace app

#include <iostream>

int main()
{
    app::C c1, c2, c3;

    c1.v.emplace_back(app::A{});
    c2.v.emplace_back(app::A{});
    c3.v.emplace_back(app::B{});

    std::cout << std::boolalpha
        << (c1 == c2) << ' '
        << (c1 == c3) << std::endl;
}
