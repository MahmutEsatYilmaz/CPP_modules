#ifndef DATA_HPP
#define DATA_HPP

#include <string>

/* Non-empty struct: just needs real data members to serialize/deserialize. */
struct Data
{
    int         id;
    std::string name;
    double      score;
};

#endif
