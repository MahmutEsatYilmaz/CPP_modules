
#include "Fixed.hpp"
#include <math.h>

Fixed::Fixed() : fixed_point(0)
{
    std::cout << "Default constructor called" << std::endl;
}

Fixed::Fixed(const int number)
{
    
    std::cout << "Int constructor called" << std::endl;
    this->fixed_point = number << fractional_bits;
}

Fixed::Fixed(const float number)
{
    std::cout << "Float constructor called" << std::endl;
    this->fixed_point = roundf(number * (1 << fractional_bits));
}

Fixed::Fixed(const Fixed &copy)
{
    std::cout << "Copy constructor called" << std::endl;
    *this = copy;
}

Fixed::~Fixed()
{
    std::cout << "Destructor called" << std::endl;
}

Fixed &Fixed::operator=(const Fixed &copy)
{
    std::cout << "Copy assignment operator called" << std::endl;
    if (this != &copy)
        this->fixed_point = copy.getRawBits();
    return *this;
}

int Fixed::getRawBits(void) const
{
    return this->fixed_point;
}

void Fixed::setRawBits(int const raw)
{
    this->fixed_point = raw;
}

float Fixed::toFloat(void) const
{
    return (float)this->fixed_point / (float)(1 << fractional_bits);
}

int Fixed::toInt(void) const
{
    return this->fixed_point >> fractional_bits;
}

bool Fixed::operator>(const Fixed &cmp) const
{
    return this->getRawBits() > cmp.getRawBits();
}

bool Fixed::operator<(const Fixed &cmp) const
{
    return this->getRawBits() < cmp.getRawBits();
}

bool Fixed::operator>=(const Fixed &cmp) const
{
    return this->getRawBits() >= cmp.getRawBits();
}

bool Fixed::operator<=(const Fixed &cmp) const
{
    return this->getRawBits() <= cmp.getRawBits();
}

bool Fixed::operator==(const Fixed &cmp) const
{
    return this->getRawBits() == cmp.getRawBits();
}

bool Fixed::operator!=(const Fixed &cmp) const
{
    return this->getRawBits() != cmp.getRawBits();
}

Fixed Fixed::operator+(const Fixed &arith) const
{
    return Fixed(this->toFloat() + arith.toFloat());
}

Fixed Fixed::operator-(const Fixed &arith) const
{
    return Fixed(this->toFloat() - arith.toFloat());
}

Fixed Fixed::operator*(const Fixed &arith) const
{
    return Fixed(this->toFloat() * arith.toFloat());
}

Fixed Fixed::operator/(const Fixed &arith) const
{
    return Fixed(this->toFloat() / arith.toFloat());
}

Fixed& Fixed::operator++()
{
    this->fixed_point++;
    return *this;
}

Fixed Fixed::operator++(int)
{
    Fixed temp(*this);
    this->fixed_point++;
    return temp;
}

Fixed& Fixed::operator--()
{
    this->fixed_point--;
    return *this;
}

Fixed Fixed::operator--(int)
{
    Fixed temp(*this);
    this->fixed_point--;
    return temp;
}

Fixed& Fixed::min(Fixed &a, Fixed &b)
{
    return (a < b) ? a : b;
}

const Fixed& Fixed::min(const Fixed &a, const Fixed &b)
{
    return (a < b) ? a : b;
}

Fixed& Fixed::max(Fixed &a, Fixed &b)
{
    return (a > b) ? a : b;
}

const Fixed& Fixed::max(const Fixed &a, const Fixed &b)
{
    return (a > b) ? a : b;
}

std::ostream &operator<<(std::ostream &out, const Fixed &fixed)
{
    out << fixed.toFloat();
    return out;
}