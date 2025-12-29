
#include "Cat.hpp"

Cat::Cat()
{
    this->type = "Cat";
    std::cout << "Cat Default constructor called" << std::endl;
}

Cat::Cat(const Cat &copy) : Animal(copy)
{
    std::cout << " Cat Copy constructor called" << std::endl;
}

Cat &Cat::operator=(const Cat &copy)
{
    std::cout << "Cat assignment operator called" << std::endl;
    if(this != &copy)
        Animal::operator=(copy);
    return(*this);
}

void    Cat::makeSound() const
{
    std::cout << "Meeeoooouuuwwwwww!!!!!" << std::endl;
}

Cat::~Cat()
{
    std::cout << "Cat destructor called" << std::endl;
}