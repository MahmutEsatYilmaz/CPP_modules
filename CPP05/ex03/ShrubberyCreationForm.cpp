
#include <fstream>
#include "ShrubberyCreationForm.hpp"

ShrubberyCreationForm::ShrubberyCreationForm()
: AForm("ShrubberyCreationForm", 145, 137), target("none")
{
}

ShrubberyCreationForm::ShrubberyCreationForm(const std::string &target)
: AForm("ShrubberyCreationForm", 145, 137), target(target)
{
}

ShrubberyCreationForm::ShrubberyCreationForm(const ShrubberyCreationForm &copy)
: AForm(copy), target(copy.target)
{
}

ShrubberyCreationForm &ShrubberyCreationForm::operator=(const ShrubberyCreationForm &copy)
{
    if(this != &copy)
    {
        AForm::operator=(copy);
        this->target = copy.target;
    }
    return (*this);
}

ShrubberyCreationForm::~ShrubberyCreationForm()
{
}

void ShrubberyCreationForm::action() const
{
    std::ofstream file((this->target + "_shrubbery").c_str());

    file << "       *\n"
            "      ***\n"
            "     *****\n"
            "    *******\n"
            "       |\n";
    file.close();
}
