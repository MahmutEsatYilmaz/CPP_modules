#include "ScalarConverter.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cctype>
#include <cstdlib>
#include <climits>
#include <limits>

/*
** ScalarConverter has nothing to construct: every member below is private
** and never defined, since nothing ever calls them (the class is only ever
** used through the static convert() function).
*/
ScalarConverter::ScalarConverter() {}
ScalarConverter::ScalarConverter(const ScalarConverter &other) { (void)other; }
ScalarConverter &ScalarConverter::operator=(const ScalarConverter &other) { (void)other; return *this; }
ScalarConverter::~ScalarConverter() {}

enum LiteralType
{
    T_CHAR,
    T_INT,
    T_FLOAT,
    T_DOUBLE,
    T_INVALID
};

/* Checks the string is a full valid strtod-style number (no leftover chars). */
static bool isValidNumber(const std::string &s)
{
    if (s.empty())
        return false;
    char *end;
    std::strtod(s.c_str(), &end);
    return (*end == '\0' && end != s.c_str());
}

static bool isValidInt(const std::string &s)
{
    if (s.empty())
        return false;
    char *end;
    std::strtol(s.c_str(), &end, 10);
    return (*end == '\0' && end != s.c_str());
}

static LiteralType classify(const std::string &literal, double &value)
{
    if (literal.size() == 3 && literal[0] == '\'' && literal[2] == '\'')
    {
        value = static_cast<double>(literal[1]);
        return T_CHAR;
    }
    if (literal == "nan" || literal == "+inf" || literal == "-inf")
    {
        value = (literal == "nan") ? std::numeric_limits<double>::quiet_NaN()
              : (literal == "+inf") ? std::numeric_limits<double>::infinity()
              : -std::numeric_limits<double>::infinity();
        return T_DOUBLE;
    }
    if (literal == "nanf" || literal == "+inff" || literal == "-inff")
    {
        value = (literal == "nanf") ? std::numeric_limits<double>::quiet_NaN()
              : (literal == "+inff") ? std::numeric_limits<double>::infinity()
              : -std::numeric_limits<double>::infinity();
        return T_FLOAT;
    }
    if (!literal.empty() && literal[literal.size() - 1] == 'f')
    {
        std::string body = literal.substr(0, literal.size() - 1);
        if (isValidNumber(body))
        {
            value = std::strtod(body.c_str(), NULL);
            return T_FLOAT;
        }
        return T_INVALID;
    }
    if (isValidInt(literal))
    {
        value = std::strtod(literal.c_str(), NULL);
        return T_INT;
    }
    if (isValidNumber(literal))
    {
        value = std::strtod(literal.c_str(), NULL);
        return T_DOUBLE;
    }
    return T_INVALID;
}

static std::string formatFixed(double v)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << v;
    return oss.str();
}

static void printChar(double value)
{
    if (std::isnan(value) || std::isinf(value) ||
        value < static_cast<double>(std::numeric_limits<char>::min()) ||
        value > static_cast<double>(std::numeric_limits<char>::max()))
    {
        std::cout << "char: impossible\n";
        return;
    }
    char c = static_cast<char>(value);
    if (std::isprint(static_cast<unsigned char>(c)))
        std::cout << "char: '" << c << "'\n";
    else
        std::cout << "char: Non displayable\n";
}

static void printInt(double value)
{
    if (std::isnan(value) || std::isinf(value) ||
        value < static_cast<double>(std::numeric_limits<int>::min()) ||
        value > static_cast<double>(std::numeric_limits<int>::max()))
    {
        std::cout << "int: impossible\n";
        return;
    }
    std::cout << "int: " << static_cast<int>(value) << "\n";
}

static void printFloat(double value)
{
    float f = static_cast<float>(value);
    if (std::isnan(f))
        std::cout << "float: nanf\n";
    else if (std::isinf(f))
        std::cout << "float: " << (f < 0 ? "-inff" : "+inff") << "\n";
    else
        std::cout << "float: " << formatFixed(static_cast<double>(f)) << "f\n";
}

static void printDouble(double value)
{
    if (std::isnan(value))
        std::cout << "double: nan\n";
    else if (std::isinf(value))
        std::cout << "double: " << (value < 0 ? "-inf" : "+inf") << "\n";
    else
        std::cout << "double: " << formatFixed(value) << "\n";
}

void ScalarConverter::convert(const std::string &literal)
{
    double value = 0.0;
    LiteralType type = classify(literal, value);

    if (type == T_INVALID)
    {
        std::cout << "char: impossible\n";
        std::cout << "int: impossible\n";
        std::cout << "float: impossible\n";
        std::cout << "double: impossible\n";
        return;
    }
    printChar(value);
    printInt(value);
    printFloat(value);
    printDouble(value);
}
