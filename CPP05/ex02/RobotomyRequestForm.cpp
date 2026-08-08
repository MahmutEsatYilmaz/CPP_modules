
#include <cstdlib>
#include "RobotomyRequestForm.hpp"

RobotomyRequestForm::RobotomyRequestForm()
: AForm("RobotomyRequestForm", 72, 45), target("none")
{
}

RobotomyRequestForm::RobotomyRequestForm(const std::string &target)
: AForm("RobotomyRequestForm", 72, 45), target(target)
{
}

RobotomyRequestForm::RobotomyRequestForm(const RobotomyRequestForm &copy)
: AForm(copy), target(copy.target)
{
}

RobotomyRequestForm &RobotomyRequestForm::operator=(const RobotomyRequestForm &copy)
{
    if(this != &copy)
    {
        AForm::operator=(copy);
        this->target = copy.target;
    }
    return (*this);
}

RobotomyRequestForm::~RobotomyRequestForm()
{
}

void RobotomyRequestForm::action() const
{
    std::cout << "* DRILLING NOISES *" << std::endl;
    if(std::rand() % 2 == 0)
        std::cout << this->target << " has been robotomized successfully" << std::endl;
    else
        std::cout << "the robotomy of " << this->target << " failed" << std::endl;
}
