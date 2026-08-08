
#ifndef BUREAUCRAT_HPP
#define BUREAUCRAT_HPP

#include <iostream>
#include <exception>

class AForm;

class Bureaucrat
{
private:
    int grade;
    const std::string name;
public:
    Bureaucrat();
    Bureaucrat(const std::string _name, int _grade);
    Bureaucrat(const Bureaucrat &copy);
    Bureaucrat &operator=(const Bureaucrat &copy);
    ~Bureaucrat();

    const std::string &getName() const;
    int getGrade() const;

    void    incrementGrade();
    void    decrementGrade();
    void    signForm(AForm &form);
    void    executeForm(AForm const &form) const;
    class GradeTooHighException : public std::exception
    {
        public:
        virtual const char* what() const throw();
    };
    class GradeTooLowException : public std::exception
    {
        public:
        virtual const char* what() const throw();
    };
};

std::ostream &operator<<(std::ostream &out, const Bureaucrat &obj);
#endif
