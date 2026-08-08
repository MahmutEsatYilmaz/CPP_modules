#include "Bureaucrat.hpp"
#include "Form.hpp"

int main()
{
    try
    {
        Form f("Vacation Request", 50, 25);
        Bureaucrat boss("Boss", 1);
        Bureaucrat intern("Intern", 150);

        std::cout << f << std::endl;

        intern.signForm(f);
        std::cout << f << std::endl;

        boss.signForm(f);
        std::cout << f << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    std::cout << "----------" << std::endl;

    try
    {
        Form badGrade("Too High", 0, 50);
        std::cout << badGrade << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    try
    {
        Form badGrade("Too Low", 200, 50);
        std::cout << badGrade << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}
