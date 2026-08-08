
#include "Bureaucrat.hpp"
#include "AForm.hpp"

AForm::AForm() : name("Empty") , sign_status(false) , sign_grade(150) , sign_exec(150)
{
    std::cout << "Default constructor called" << std::endl;
}
AForm::AForm(const	std::string _name, const int	_sign_grade, const int	_sign_exec)
: name(_name) , sign_status(false) , sign_grade(_sign_grade) , sign_exec(_sign_exec)
{
    if(this->sign_exec > 150 || this->sign_grade > 150)
    {
        throw AForm::GradeTooLowException();
    }
    if(this->sign_exec < 1 || this->sign_grade < 1)
    {
        throw AForm::GradeTooHighException();
    }
    std::cout << this->name << " is created" << std::endl;
}

AForm::AForm(const AForm &copy)
: name(copy.name) , sign_status(copy.sign_status) , sign_grade(copy.sign_grade) , sign_exec(copy.sign_exec)
{
    std::cout << "Copy constructor is called" << std::endl;
}

AForm &AForm::operator=(const AForm &copy)
{
    if(this != &copy)
    {
        this->sign_status = copy.sign_status;
    }
    std::cout << "Assignment operator is called" << std::endl;
    return (*this);
}

AForm::~AForm()
{
}

const std::string &AForm::getName() const
{
    return this->name;
}

const int &AForm::getSign_Grade() const
{
    return this->sign_grade;
}
const int &AForm::getSign_exec() const
{
    return this->sign_exec;
}

bool AForm::getSign_Status() const
{
    return this->sign_status;
}

void AForm::beSigned(const Bureaucrat &approver)
{
    if(approver.getGrade() > this->sign_grade)
    {
        throw AForm::GradeTooLowException();
    }
    this->sign_status = true;
}

void AForm::execute(Bureaucrat const & executor) const
{
    if(!this->sign_status)
    {
        throw AForm::FormNotSignedException();
    }
    if(executor.getGrade() > this->sign_exec)
    {
        throw AForm::GradeTooLowException();
    }
    this->action();
}

const char* AForm::GradeTooHighException::what() const throw()
{
    return "grade is too high";
}

const char* AForm::GradeTooLowException::what() const throw()
{
    return "grade is too low";
}

const char* AForm::FormNotSignedException::what() const throw()
{
    return "the form is not signed";
}

std::ostream &operator<<(std::ostream &out, const AForm &obj)
{
    out << obj.getName() << ", AForm sign grade " <<  obj.getSign_Grade()
    << ", AForm sign exec " << obj.getSign_exec() << ", AForm sign status " << obj.getSign_Status();
    return out;
}
