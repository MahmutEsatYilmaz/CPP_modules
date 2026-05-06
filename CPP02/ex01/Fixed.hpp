
#ifndef FIXED_HPP
#define FIXED_HPP
#include <iostream>

class Fixed
{
private:
    int                 fixed_point;
    static const int    fractional_bits = 8;
public:
    Fixed();
    ~Fixed();
    Fixed(int number);
    Fixed(const float number);
    Fixed(const Fixed &copy);
    Fixed&  operator=(const Fixed& load);
    float   toFloat(void) const;
    int     toInt(void) const;
};

std::ostream &operator<<(std::ostream &out, const Fixed &fixed_point);

#endif