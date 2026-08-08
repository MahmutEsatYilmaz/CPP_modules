#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include "easyfind.hpp"

template <typename T>
static void testFound(T &container, int value, const char *label)
{
	try
	{
		typename T::iterator it = easyfind(container, value);
		std::cout << label << ": found " << *it << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cout << label << ": " << e.what() << std::endl;
	}
}

int main()
{
	std::vector<int> v;
	v.push_back(1);
	v.push_back(4);
	v.push_back(9);
	v.push_back(16);

	std::list<int> l;
	l.push_back(2);
	l.push_back(3);
	l.push_back(5);

	std::deque<int> d;
	d.push_back(10);
	d.push_back(20);
	d.push_back(30);

	testFound(v, 9, "vector (existing)");
	testFound(v, 42, "vector (missing)");

	testFound(l, 3, "list (existing)");
	testFound(l, 99, "list (missing)");

	testFound(d, 20, "deque (existing)");
	testFound(d, -1, "deque (missing)");

	return (0);
}
