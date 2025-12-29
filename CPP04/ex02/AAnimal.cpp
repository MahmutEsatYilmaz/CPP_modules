
#include "AAnimal.hpp"

AAnimal::AAnimal()
{
    this->type = "AAnimal";
    std::cout << "AAnimal Default constructor called" << std::endl;
}

AAnimal::AAnimal(const AAnimal &copy)
{
    std::cout << "AAnimal Copy constructor called" << std::endl;
    *this = copy;
}

AAnimal &AAnimal::operator=(const AAnimal &copy)
{
    std::cout << "AAnimal assignment operator called" << std::endl;
    if(this != &copy)
        this->type = copy.type;
    return(*this);
}

std::string  AAnimal::getType() const
{
    return(this->type);
}

AAnimal::~AAnimal()
{
    std::cout << "AAnimal destructor called" << std::endl;
}