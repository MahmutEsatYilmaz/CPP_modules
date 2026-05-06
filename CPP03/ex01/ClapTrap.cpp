
#include "ClapTrap.hpp"

ClapTrap::ClapTrap(std::string Name)
{
    this->Name = Name;
    this->Attack_damage = 0;
    this->Energy_points = 10;
    this->Hit_points = 10;
    std::cout << this->Name << " is created" << std::endl;
}

ClapTrap::~ClapTrap()
{
    std::cout << this->Name << " is detroyed" << std::endl;
}

ClapTrap::ClapTrap(const ClapTrap &copy)
{
    this->Name = copy.Name;
    this->Hit_points = copy.Hit_points;
    this->Energy_points = copy.Energy_points;
    this->Attack_damage = copy.Attack_damage;
    std::cout << "Copy constsructure is called" << std::endl;
}

ClapTrap &ClapTrap::operator=(const ClapTrap &copy)
{
    this->Name = copy.Name;
    this->Hit_points = copy.Hit_points;
    this->Energy_points = copy.Energy_points;
    this->Attack_damage = copy.Attack_damage;
    std::cout << "Assignment operator is called" << std::endl;
    return (*this);
}

void    ClapTrap::attack(const std::string &target)
{
    if(Energy_points <= 0 || this->Hit_points <= 0)
    {
        std::cout << this->Name <<  " is no hit points or energy points left." << std::endl;
        return;
    }
    this->Energy_points--;
    std::cout << "ClapTrap " << this->Name << " attacks " << target;
    std::cout << ", causing " << this->Attack_damage << " points of damage" << std::endl;
}

void    ClapTrap::takeDamage(unsigned int amount)
{
    this->Hit_points -= amount;
    if(this->Hit_points <= 0)
        return;
    std::cout << "ClapTrap " << this->Name << " takes " << amount << " points of damage ";
    std::cout << "HP: " << this->Hit_points << std::endl;
}

void    ClapTrap::beRepaired(unsigned int amount)
{
    if(Energy_points <= 0 || this->Hit_points <= 0)
    {
        std::cout << this->Name <<  " is no hit points or energy points left." << std::endl;
        return;
    }
    this->Energy_points--;
    this->Hit_points += amount;
    std::cout << this->Name << " is repaired HP: " << this->Hit_points << std::endl;
}