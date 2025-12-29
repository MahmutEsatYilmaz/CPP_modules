
#ifndef WRONGCAT_HPP
#define WRONGCAT_HPP

#include "WrongAnimal.hpp"

class WrongCat : public WrongAnimal
{
public:
    WrongCat();
    WrongCat(const WrongCat &copy);
    void    makeSound() const;
    WrongCat &operator=(const WrongCat &copy);
    ~WrongCat();
};

#endif
