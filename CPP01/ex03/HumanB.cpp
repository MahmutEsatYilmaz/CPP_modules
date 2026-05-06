
#include "HumanB.hpp"

HumanB::HumanB(std::string name)
{
    this->name = name;
}

void    HumanB::setWeapon(Weapon weapon)
{
    this->B = weapon;
}

void    HumanB::attack()
{
    std::cout << this->name << " attacks with their " << this->B.getType();
}