#include "identify.hpp"
#include <cstdlib>
#include <ctime>

int main()
{
    std::srand(static_cast<unsigned int>(std::time(NULL)));

    for (int i = 0; i < 8; ++i)
    {
        Base *p = generate();
        identify(p);
        identify(*p);
        delete p;
    }
    return 0;
}
