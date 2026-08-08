
#include "PresidentialPardonForm.hpp"

PresidentialPardonForm::PresidentialPardonForm()
: AForm("PresidentialPardonForm", 25, 5), target("none")
{
}

PresidentialPardonForm::PresidentialPardonForm(const std::string &target)
: AForm("PresidentialPardonForm", 25, 5), target(target)
{
}

PresidentialPardonForm::PresidentialPardonForm(const PresidentialPardonForm &copy)
: AForm(copy), target(copy.target)
{
}

PresidentialPardonForm &PresidentialPardonForm::operator=(const PresidentialPardonForm &copy)
{
    if(this != &copy)
    {
        AForm::operator=(copy);
        this->target = copy.target;
    }
    return (*this);
}

PresidentialPardonForm::~PresidentialPardonForm()
{
}

void PresidentialPardonForm::action() const
{
    std::cout << this->target << " has been pardoned by Zaphod Beeblebrox" << std::endl;
}
