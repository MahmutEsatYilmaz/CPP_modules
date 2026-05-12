
#ifndef FORM_HPP
#define FORM_HPP

#include <iostream>
#include "Bureaucrat.hpp"

class Form
{
private:
	const		std::string name;
	bool		sign_status;
	const int	sign_grade;
	const int	sign_exec;
public:
	Form();
	Form(const	std::string _name, const int	_sign_grade, const int	_sign_exec);
	Form(const Form &copy);
	Form &operator=(const Form	&copy);
	~Form();

	const std::string	&getName() const;
	const int	&getSign_Grade() const;
	const int	&getSign_exec() const;
	bool	getSign_Status() const;

	void	beSigned(const Bureaucrat &approver);
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