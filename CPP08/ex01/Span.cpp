#include "Span.hpp"
#include <algorithm>

Span::Span(unsigned int n) : _maxSize(n)
{
	this->_numbers.reserve(n);
}

Span::Span(const Span &other) : _maxSize(other._maxSize), _numbers(other._numbers)
{
}

Span &Span::operator=(const Span &other)
{
	if (this != &other)
	{
		this->_maxSize = other._maxSize;
		this->_numbers = other._numbers;
	}
	return (*this);
}

Span::~Span()
{
}

void Span::addNumber(int n)
{
	if (this->_numbers.size() >= this->_maxSize)
		throw Span::FullException();
	this->_numbers.push_back(n);
}

int Span::shortestSpan() const
{
	if (this->_numbers.size() < 2)
		throw Span::NotEnoughNumbersException();

	std::vector<int> sorted(this->_numbers);
	std::sort(sorted.begin(), sorted.end());

	int shortest = sorted[1] - sorted[0];
	for (std::vector<int>::size_type i = 2; i < sorted.size(); ++i)
	{
		int diff = sorted[i] - sorted[i - 1];
		if (diff < shortest)
			shortest = diff;
	}
	return (shortest);
}

int Span::longestSpan() const
{
	if (this->_numbers.size() < 2)
		throw Span::NotEnoughNumbersException();

	std::vector<int>::const_iterator minIt = std::min_element(this->_numbers.begin(), this->_numbers.end());
	std::vector<int>::const_iterator maxIt = std::max_element(this->_numbers.begin(), this->_numbers.end());
	return (*maxIt - *minIt);
}

const char *Span::FullException::what() const throw()
{
	return ("Span: cannot add number, span is already full");
}

const char *Span::NotEnoughNumbersException::what() const throw()
{
	return ("Span: not enough numbers stored to compute a span");
}
