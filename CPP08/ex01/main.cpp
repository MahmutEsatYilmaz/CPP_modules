#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Span.hpp"

static void subjectExample()
{
	Span sp = Span(5);

	sp.addNumber(6);
	sp.addNumber(3);
	sp.addNumber(17);
	sp.addNumber(9);
	sp.addNumber(11);

	std::cout << sp.shortestSpan() << std::endl;
	std::cout << sp.longestSpan() << std::endl;
}

static void extraTests()
{
	Span sp(3);
	try
	{
		sp.addNumber(1);
		sp.addNumber(2);
		sp.addNumber(3);
		sp.addNumber(4);
	}
	catch (const std::exception &e)
	{
		std::cout << "expected overflow: " << e.what() << std::endl;
	}

	Span empty(2);
	try
	{
		empty.addNumber(42);
		empty.shortestSpan();
	}
	catch (const std::exception &e)
	{
		std::cout << "expected underflow: " << e.what() << std::endl;
	}

	std::vector<int> range;
	range.push_back(1);
	range.push_back(2);
	range.push_back(3);
	Span rangeSpan(5);
	rangeSpan.addNumbers(range.begin(), range.end());
	std::cout << "range span shortest: " << rangeSpan.shortestSpan() << std::endl;

	try
	{
		Span tooSmall(2);
		tooSmall.addNumbers(range.begin(), range.end());
	}
	catch (const std::exception &e)
	{
		std::cout << "expected addNumbers overflow: " << e.what() << std::endl;
	}
}

static void largeSpanTest()
{
	const unsigned int count = 10000;
	std::vector<int> values;
	values.reserve(count);

	std::srand(static_cast<unsigned int>(std::time(NULL)));
	for (unsigned int i = 0; i < count; ++i)
		values.push_back(std::rand());

	Span big(count);
	big.addNumbers(values.begin(), values.end());

	std::cout << "large span (" << count << " numbers) shortest: " << big.shortestSpan() << std::endl;
	std::cout << "large span (" << count << " numbers) longest: " << big.longestSpan() << std::endl;
}

int main()
{
	subjectExample();
	extraTests();
	largeSpanTest();
	return (0);
}
