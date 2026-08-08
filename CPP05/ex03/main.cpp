#include <ctime>
#include <cstdlib>
#include "Bureaucrat.hpp"
#include "Intern.hpp"
#include "AForm.hpp"

int main()
{
    std::srand(static_cast<unsigned int>(std::time(NULL)));

    Intern intern;
    Bureaucrat boss("Boss", 1);

    std::string formNames[4] = {
        "shrubbery creation",
        "robotomy request",
        "presidential pardon",
        "coffee making"
    };

    for (int i = 0; i < 4; ++i)
    {
        std::cout << "----- " << formNames[i] << " -----" << std::endl;
        AForm *form = intern.makeForm(formNames[i], "Bender");
        if (form == NULL)
            continue;
        boss.signForm(*form);
        boss.executeForm(*form);
        delete form;
    }

    return 0;
}
