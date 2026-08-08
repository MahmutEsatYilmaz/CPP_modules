
#include "Bureaucrat.hpp"
#include "Form.hpp"

Form::Form() : name("Empty") , sign_status(false) , sign_grade(150) , sign_exec(150)
{
    std::cout << "Default constructor called" << std::endl;
}
Form::Form(const	std::string _name, const int	_sign_grade, const int	_sign_exec)
: name(_name) , sign_status(false) , sign_grade(_sign_grade) , sign_exec(_sign_exec)
{
    if(this->sign_exec > 150 || this->sign_grade > 150)
    {
        throw Form::GradeTooLowException();
    }
    if(this->sign_exec < 1 || this->sign_grade < 1)
    {
        throw Form::GradeTooHighException();
    }
    std::cout << this->name << " is created" << std::endl;
}

Form::Form(const Form &copy)
: name(copy.name) , sign_status(copy.sign_status) , sign_grade(copy.sign_grade) , sign_exec(copy.sign_exec)
{
    std::cout << "Copy constructor is called" << std::endl;
}

Form &Form::operator=(const Form &copy)
{
    if(this != &copy)
    {
        this->sign_status = copy.sign_status;
    }
    std::cout << "Assignment operator is called" << std::endl;
    return (*this);  
}

Form::~Form()
{
}

const std::string &Form::getName() const
{
    return this->name;
}

const int &Form::getSign_Grade() const
{
    return this->sign_grade;
}
const int &Form::getSign_exec() const
{
    return this->sign_exec;
}

bool Form::getSign_Status() const
{
    return this->sign_status;
}

void Form::beSigned(const Bureaucrat &approver)
{
    if(approver.getGrade() > this->sign_grade)
    {
        throw Form::GradeTooLowException();
    }
    this->sign_status = true;
}

const char* Form::GradeTooHighException::what() const throw() {
    return "grade is too high";
}

const char* Form::GradeTooLowException::what() const throw() {
    return "grade is too low";
}

std::ostream &operator<<(std::ostream &out, const Form &obj)
{
    out << obj.getName() << ", Form sign grade " <<  obj.getSign_Grade()
    << ", Form sign exec " << obj.getSign_exec() << ", Form sign status " << obj.getSign_Status();
    return out;
}