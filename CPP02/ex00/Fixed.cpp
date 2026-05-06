
#include "Fixed.hpp"

Fixed::Fixed(const Fixed &a)
{
    this->fixed_point = a.getRawBits();
    std::cout << "Copy constructor called" << std::endl;
}


Fixed::Fixed()
{
    this->fixed_point = 0;
    std::cout << "Default constructor called" << std::endl;
}

Fixed& Fixed::operator=(const Fixed& load)
{
    std::cout << "Copy assignment operator called" << std::endl;
    if (this != &load)
        this->fixed_point = load.getRawBits();
    return *this;
}

int Fixed::getRawBits() const
{
    std::cout << "getRawBits member function called" << std::endl;
    return (this->fixed_point);
}

void    Fixed::setRawBits(int const raw)
{
     std::cout << "setRawBits member function called" << std::endl;
     this->fixed_point = raw;
}

Fixed::~Fixed()
{
    std::cout <<"Destructor called" << std::endl;
}