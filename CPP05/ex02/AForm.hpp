
#ifndef AFORM_HPP
#define AFORM_HPP

#include <iostream>

class Bureaucrat;

class AForm
{
private:
	const		std::string name;
	bool		sign_status;
	const int	sign_grade;
	const int	sign_exec;
public:
	AForm();
	AForm(const	std::string _name, const int	_sign_grade, const int	_sign_exec);
	AForm(const AForm &copy);
	AForm &operator=(const AForm	&copy);
	virtual ~AForm();

	const std::string	&getName() const;
	const int	&getSign_Grade() const;
	const int	&getSign_exec() const;
	bool	getSign_Status() const;

	void	beSigned(const Bureaucrat &approver);
	void	execute(Bureaucrat const & executor) const;
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
	class FormNotSignedException : public std::exception {
        public: virtual const char* what() const throw();
    };
protected:
	virtual void	action() const = 0;
};
std::ostream &operator<<(std::ostream &out, const AForm &obj);
#endif
