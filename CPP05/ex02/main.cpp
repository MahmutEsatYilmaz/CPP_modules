#include <ctime>
#include <cstdlib>
#include "Bureaucrat.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"

int main()
{
    std::srand(static_cast<unsigned int>(std::time(NULL)));

    Bureaucrat boss("Boss", 1);
    Bureaucrat manager("Manager", 100);
    Bureaucrat intern("Intern", 150);

    ShrubberyCreationForm shrub("garden");
    RobotomyRequestForm robot("Bender");
    PresidentialPardonForm pardon("Fry");

    std::cout << "----- signing -----" << std::endl;
    intern.signForm(shrub);
    manager.signForm(shrub);
    std::cout << shrub << std::endl;

    std::cout << "----- executing an unsigned form -----" << std::endl;
    boss.executeForm(robot);

    std::cout << "----- signing then executing with too low a grade -----" << std::endl;
    boss.signForm(robot);
    manager.executeForm(robot);
    boss.executeForm(robot);

    std::cout << "----- presidential pardon -----" << std::endl;
    boss.signForm(pardon);
    intern.executeForm(pardon);
    boss.executeForm(pardon);

    std::cout << "----- shrubbery creation -----" << std::endl;
    manager.executeForm(shrub);

    return 0;
}
