
#ifndef AANIMAL_HPP
#define AANIMAL_HPP

#include <iostream>

class AAnimal
{
protected:
    std::string type;
public:
    AAnimal();
    AAnimal(const AAnimal &copy);
    AAnimal &operator=(const AAnimal &copy);
    std::string getType() const;
    virtual ~AAnimal();
    virtual void makeSound() const = 0;
};

#endif
