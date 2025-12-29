
#include "Dog.hpp"

Dog::Dog()
{
    std::cout << "Dog Default constructor called" << std::endl;
    this->type = "Dog";
    this->brain = new Brain();
}

Dog::Dog(const Dog &copy) : AAnimal(copy)
{
    std::cout << "Dog Copy constructor called" << std::endl;
    this->brain = new Brain(*(copy.brain));
}

Dog &Dog::operator=(const Dog &copy)
{
    std::cout << "Dog assignment operator called" << std::endl;
    if(this != &copy)
    {
        delete this->brain;
        this->brain = new Brain(*(copy.brain));
        AAnimal::operator=(copy);
    }
    return(*this);
}

void    Dog::makeSound() const
{
    std::cout << "!!!Hhhrrrrrrrrrrrrr!!!" << std::endl;
}

Dog::~Dog()
{
    delete brain;
    std::cout << "Dog destructor called" << std::endl;
}