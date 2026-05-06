
#include "Fixed.hpp"
#include <cmath>

Fixed::Fixed()
{
    this->fixed_point = 0;
    std::cout << "Default constructor called" << std::endl;
}

Fixed::Fixed(const Fixed &copy)
{
    std::cout << "Copy constructor called"  << std::endl;
    this->fixed_point = copy.fixed_point;
}

Fixed::Fixed(int number)
{
    std::cout << "Int constructor called" << std::endl;
    this->fixed_point = number << this->fractional_bits;
}

Fixed::Fixed(const float number)
{
    std::cout << "Float constructor called" << std::endl;
    this->fixed_point = roundf(number * (1 << fractional_bits));
}

float Fixed::toFloat() const
{
   return (static_cast<float>(this->fixed_point) / (1 << fractional_bits));
}

int Fixed::toInt() const
{
    return (this->fixed_point >> fractional_bits);
}

Fixed::~Fixed()
{
    std::cout << "Destructor called" << std::endl;
}

Fixed& Fixed::operator=(const Fixed& load)
{
    std::cout << "Copy assignment operator called" << std::endl;
    if (this != &load)
        this->fixed_point = load.fixed_point;
    return *this;
}

std::ostream &operator<<(std::ostream &out, const Fixed &fixed_point)
{
    out << fixed_point.toFloat();
    return (out);
}