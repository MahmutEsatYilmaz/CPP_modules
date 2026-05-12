#include "Bureaucrat.hpp"

int main()
{
    try
    {
        Bureaucrat s1("Mahmut", 2);

        std::cout << s1 << std::endl;
        s1.incrementGrade();
        std::cout << s1 << std::endl;
        s1.incrementGrade();
        std::cout << "!!!!!!!!!!" << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    try
    {
        Bureaucrat s2("Esat", 149);

        std::cout << s2 << std::endl;
        s2.decrementGrade();
        std::cout << s2 << std::endl;
        s2.decrementGrade();
        std::cout << "**********" << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    try
    {
        Bureaucrat s3("yilmaz", 151);
        std::cout << "----------" << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    try
    {
        Bureaucrat s4("fatih", 0);
        std::cout << "==========" << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}