
#include "Animal.hpp"

Animal::Animal()
{
    this->type = "Animal";
    std::cout << "Animal Default constructor called" << std::endl;
}

Animal::Animal(const Animal &copy)
{
    std::cout << "Animal Copy constructor called" << std::endl;
    *this = copy;
}

Animal &Animal::operator=(const Animal &copy)
{
    std::cout << "Animal assignment operator called" << std::endl;
    if(this != &copy)
        this->type = copy.type;
    return(*this);
}

std::string  Animal::getType() const
{
    return(this->type);
}

void    Animal::makeSound() const
{
    std::cout << "Some Animal noises..." << std::endl;
}

Animal::~Animal()
{
    std::cout << "Animal destructor called" << std::endl;
}