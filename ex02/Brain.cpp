#include "Brain.hpp"

Brain::Brain()
{
    std::cout << "Brain Default constructor called" << std::endl;
}

Brain::Brain(const Brain &copy)
{
    std::cout << "Brain Copy constructor called" << std::endl;
    int i = 0;
    while (i < 100)
    {
        this->idea[i] = copy.idea[i];
        i++;
    }
}

Brain &Brain::operator=(const Brain &copy)
{
    std::cout << "Brain assignment operator called" << std::endl;
    if(this != &copy)
    {
        int i = 0;
        while (i < 100)
        {
            this->idea[i] = copy.idea[i];
            i++;
        }
    }
    return(*this);
}

Brain::~Brain()
{
    std::cout << "Brain destructor called" << std::endl;
}