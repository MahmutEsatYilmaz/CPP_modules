
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
    Fixed(const int number);
    Fixed(const float number);
    Fixed(const Fixed &copy);
    Fixed&  operator=(const Fixed& copy);
    float   toFloat(void) const;
    int     toInt(void) const;
    int     getRawBits() const;
    void    setRawBits(int const raw);

    bool    operator>(const Fixed &cmp) const;
    bool    operator<(const Fixed &cmp) const;
    bool    operator>=(const Fixed &cmp) const;
    bool    operator<=(const Fixed &cmp) const;
    bool    operator==(const Fixed &cmp) const;
    bool    operator!=(const Fixed &cmp) const;

    Fixed   operator+(const Fixed &arith) const;
    Fixed   operator-(const Fixed &arith) const;
    Fixed   operator*(const Fixed &arith) const;
    Fixed   operator/(const Fixed &arith) const;

    Fixed&  operator++();
    Fixed   operator++(int);
    Fixed&  operator--();
    Fixed   operator--(int);

    static Fixed& min(Fixed &a, Fixed &b);
    static const Fixed& min(const Fixed &a, const Fixed &b);
    static Fixed& max(Fixed &a, Fixed &b);
    static const Fixed& max(const Fixed &a, const Fixed &b);
};

std::ostream &operator<<(std::ostream &out, const Fixed &fixed_point);

#endif
