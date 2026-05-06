
#ifndef ZOMBİE_HPP
#define ZOMBİE_HPP
#include <iostream>

class Zombie
{
private:
    std::string name;
public:
    Zombie(std::string name);
    void    announce();
    ~Zombie();
};

Zombie *newZombie(std::string name);
void    randonChump(std::string name);


#endif