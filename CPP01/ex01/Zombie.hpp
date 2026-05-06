
#ifndef ZOMBİE_HPP
#define ZOMBİE_HPP
#include <iostream>

class Zombie
{
private:
    std::string name;
public:
    Zombie(std::string name);
    ~Zombie();
    Zombie();
    void    set_name(std::string name);
    void    announce();
};

Zombie  *zombieHorde(int N, std::string name);

#endif