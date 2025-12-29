
#include "WrongAnimal.hpp"

WrongAnimal::WrongAnimal()
{
    this->type = "WrongAnimal";
    std::cout << "WrongAnimal Default constructor called" << std::endl;
}

WrongAnimal::WrongAnimal(const WrongAnimal &copy)
{
    std::cout << "WrongAnimal Copy constructor called" << std::endl;
    *this = copy;
}

WrongAnimal &WrongAnimal::operator=(const WrongAnimal &copy)
{
    std::cout << "WrongAnimal assignment operator called" << std::endl;
    if(this != &copy)
        this->type = copy.type;
    return(*this);
}

void    WrongAnimal::makeSound() const
{
    std::cout << "<Wrong> Some Animal noises..." << std::endl;
}

std::string WrongAnimal::getType() const
{
    return (this->type);
}

WrongAnimal::~WrongAnimal()
{
    std::cout << "WrongAnimal destructor called" << std::endl;
}