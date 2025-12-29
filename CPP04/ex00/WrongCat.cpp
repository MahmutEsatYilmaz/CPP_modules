
#include "WrongCat.hpp"

WrongCat::WrongCat()
{
    this->type = "WrongCat";
    std::cout << "WrongCat Default constructor called" << std::endl;
}

WrongCat::WrongCat(const WrongCat &copy) : WrongAnimal(copy)
{
    std::cout << " WrongCat Copy constructor called" << std::endl;
}

WrongCat &WrongCat::operator=(const WrongCat &copy)
{
    std::cout << "WrongCat assignment operator called" << std::endl;
    if(this != &copy)
        WrongAnimal::operator=(copy);
    return(*this);
}

void    WrongCat::makeSound() const
{
    std::cout << "Meeeoooouuuwwwwww!!!!!" << std::endl;
}

WrongCat::~WrongCat()
{
    std::cout << "WrongCat destructor called" << std::endl;
}