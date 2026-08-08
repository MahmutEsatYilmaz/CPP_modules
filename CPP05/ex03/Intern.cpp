
#include <iostream>
#include "Intern.hpp"
#include "AForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"

Intern::Intern()
{
}

Intern::Intern(const Intern &copy)
{
    (void)copy;
}

Intern &Intern::operator=(const Intern &copy)
{
    (void)copy;
    return (*this);
}

Intern::~Intern()
{
}

static AForm *createShrubbery(const std::string &target)
{
    return new ShrubberyCreationForm(target);
}

static AForm *createRobotomy(const std::string &target)
{
    return new RobotomyRequestForm(target);
}

static AForm *createPardon(const std::string &target)
{
    return new PresidentialPardonForm(target);
}

AForm *Intern::makeForm(const std::string &formName, const std::string &target) const
{
    typedef AForm *(*FormFactory)(const std::string &);

    struct FormEntry
    {
        std::string name;
        FormFactory factory;
    };

    static const FormEntry forms[] = {
        { "shrubbery creation", &createShrubbery },
        { "robotomy request", &createRobotomy },
        { "presidential pardon", &createPardon }
    };
    static const size_t formsCount = sizeof(forms) / sizeof(forms[0]);

    for (size_t i = 0; i < formsCount; ++i)
    {
        if (forms[i].name == formName)
        {
            std::cout << "Intern creates " << formName << std::endl;
            return forms[i].factory(target);
        }
    }
    std::cerr << "Error: form \"" << formName << "\" does not exist" << std::endl;
    return NULL;
}
