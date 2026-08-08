#ifndef SPAN_HPP
#define SPAN_HPP

#include <vector>
#include <exception>

class Span
{
public:
	explicit Span(unsigned int n);
	Span(const Span &other);
	Span &operator=(const Span &other);
	~Span();

	void addNumber(int n);

	template <typename InputIterator>
	void addNumbers(InputIterator first, InputIterator last)
	{
		typename std::vector<int>::size_type count = 0;
		for (InputIterator it = first; it != last; ++it)
			++count;
		if (this->_numbers.size() + count > this->_maxSize)
			throw Span::FullException();
		this->_numbers.insert(this->_numbers.end(), first, last);
	}

	int shortestSpan() const;
	int longestSpan() const;

	class FullException : public std::exception
	{
	public:
		virtual const char *what() const throw();
	};

	class NotEnoughNumbersException : public std::exception
	{
	public:
		virtual const char *what() const throw();
	};

private:
	Span();

	unsigned int _maxSize;
	std::vector<int> _numbers;
};

#endif
