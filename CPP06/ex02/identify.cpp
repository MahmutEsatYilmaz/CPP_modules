#include "identify.hpp"
#include "A.hpp"
#include "B.hpp"
#include "C.hpp"
#include <cstdlib>
#include <iostream>

Base *generate(void)
{
    switch (std::rand() % 3)
    {
        case 0:
            return new A();
        case 1:
            return new B();
        default:
            return new C();
    }
}

void identify(Base *p)
{
    if (dynamic_cast<A *>(p))
        std::cout << "A\n";
    else if (dynamic_cast<B *>(p))
        std::cout << "B\n";
    else if (dynamic_cast<C *>(p))
        std::cout << "C\n";
    else
        std::cout << "unknown\n";
}

/*
** dynamic_cast on a reference throws std::bad_cast on failure, but naming
** that type requires <typeinfo>, which this exercise forbids. catch (...)
** catches it (or anything else) without ever naming the type.
*/
void identify(Base &p)
{
    try
    {
        (void)dynamic_cast<A &>(p);
        std::cout << "A\n";
        return;
    }
    catch (...)
    {
    }
    try
    {
        (void)dynamic_cast<B &>(p);
        std::cout << "B\n";
        return;
    }
    catch (...)
    {
    }
    try
    {
        (void)dynamic_cast<C &>(p);
        std::cout << "C\n";
        return;
    }
    catch (...)
    {
        std::cout << "unknown\n";
    }
}
