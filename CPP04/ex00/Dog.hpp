
#ifndef DOG_HPP

#define DOG_HPP

#include "Animal.hpp"

class Dog : public Animal
{
public:
    Dog();
    Dog(const Dog &copy);
    void    makeSound() const;
    Dog &operator=(const Dog &copy);
    ~Dog();
};

#endif