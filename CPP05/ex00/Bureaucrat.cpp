
#include "Bureaucrat.hpp"

Bureaucrat::Bureaucrat() : grade(150), name("none")
{
    std::cout << "Default constructor called" << std::endl;
}

Bureaucrat::Bureaucrat(const std::string _name, int _grade) : name(_name)
{
    this->grade = _grade;
    if(grade > 150)
    {
        throw Bureaucrat::GradeTooLowException();
    }
    if(grade < 1)
    {
        throw Bureaucrat::GradeTooHighException();
    }
    std::cout << this->name << " is created" << std::endl;
}

Bureaucrat::Bureaucrat(const Bureaucrat &copy) : name(copy.name)
{
    this->grade = copy.grade;
    std::cout << "Copy constructor is called" << std::endl;
}
Bureaucrat::~Bureaucrat()
{
    std::cout << this->name << " is destroyed" << std::endl;
}

Bureaucrat &Bureaucrat::operator=(const Bureaucrat &copy)
{
    if(this != &copy)
    {
        this->grade = copy.grade;
    }
    std::cout << "Assignment operator is called" << std::endl;
    return (*this);
}
const std::string &Bureaucrat::getName() const
{
    return (this->name);
}
int Bureaucrat::getGrade() const
{
    return(this->grade);
}

void    Bureaucrat::decrementGrade()
{
    if(grade >= 150)
    {
        throw Bureaucrat::GradeTooLowException();
    }
    ++grade;
}

void    Bureaucrat::incrementGrade()
{
    if(grade <= 1)
    {
        throw Bureaucrat::GradeTooHighException();
    }
    --grade;
}

const char* Bureaucrat::GradeTooHighException::what() const throw() {
    return "grade is too high";
}

const char* Bureaucrat::GradeTooLowException::what() const throw() {
    return "grade is too low";
}

std::ostream &operator<<(std::ostream &out, const Bureaucrat &obj) {
    out << obj.getName() << ", bureaucrat grade " << obj.getGrade() << ".";
    return out;
}

