
#ifndef BRAIN_HPP
#define BRAIN_HPP

#include <iostream>

class Brain
{

public:
    Brain();
    std::string idea[100];
    Brain(const Brain &copy);
    Brain &operator=(const Brain &copy);

    ~Brain();
};

#endif