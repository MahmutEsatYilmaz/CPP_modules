
#include "Cat.hpp"

Cat::Cat()
{
    std::cout << "Cat Default constructor called" << std::endl;
    this->type = "Cat";
    this->brain = new Brain();
}

Cat::Cat(const Cat &copy) : Animal(copy)
{
    std::cout << " Cat Copy constructor called" << std::endl;
    this->brain = new Brain(*(copy.brain));
}

Cat &Cat::operator=(const Cat &copy)
{
    std::cout << "Cat assignment operator called" << std::endl;
    if(this != &copy)
    {
        delete this->brain;
        this->brain = new Brain(*(copy.brain));
        Animal::operator=(copy);
    }
    return(*this);
}

void    Cat::makeSound() const
{
    std::cout << "Meeeoooouuuwwwwww!!!!!" << std::endl;
}

Cat::~Cat()
{
    delete brain;
    std::cout << "Cat destructor called" << std::endl;
}