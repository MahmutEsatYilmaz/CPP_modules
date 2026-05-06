
#include "HumanA.hpp"
#include "Weapon.hpp"

HumanA::HumanA(std::string name, Weapon &weapon) : name(name), A(weapon)
{
}

void    HumanA::attack()
{
    std::cout << this->name << " attacks with their " << this->A.getType();
}