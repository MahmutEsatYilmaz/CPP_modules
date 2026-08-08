#include <iostream>
#include "Array.hpp"

int main(void)
{
	std::cout << "-- default constructor (empty array) --" << std::endl;
	Array<int> empty;
	std::cout << "empty.size() = " << empty.size() << std::endl;

	std::cout << std::endl << "-- Array<int> a(5), elements value-initialized to 0 --" << std::endl;
	Array<int> a(5);
	std::cout << "a.size() = " << a.size() << std::endl;
	for (unsigned int i = 0; i < a.size(); ++i)
		std::cout << "a[" << i << "] = " << a[i] << std::endl;

	std::cout << std::endl << "-- modifying elements via operator[] --" << std::endl;
	for (unsigned int i = 0; i < a.size(); ++i)
		a[i] = (i + 1) * 10;
	for (unsigned int i = 0; i < a.size(); ++i)
		std::cout << "a[" << i << "] = " << a[i] << std::endl;

	std::cout << std::endl << "-- copy constructor, deep copy check --" << std::endl;
	Array<int> b(a);
	b[0] = 999;
	std::cout << "a[0] = " << a[0] << " (original, unchanged)" << std::endl;
	std::cout << "b[0] = " << b[0] << " (copy, modified)" << std::endl;

	std::cout << std::endl << "-- copy assignment operator, deep copy check --" << std::endl;
	Array<int> c;
	c = a;
	c[1] = 777;
	std::cout << "a[1] = " << a[1] << " (original, unchanged)" << std::endl;
	std::cout << "c[1] = " << c[1] << " (assigned copy, modified)" << std::endl;
	std::cout << "c.size() = " << c.size() << std::endl;

	std::cout << std::endl << "-- out of bounds access --" << std::endl;
	try
	{
		std::cout << a[a.size()] << std::endl;
	}
	catch (std::exception const &e)
	{
		std::cout << "caught exception: " << e.what() << std::endl;
	}

	std::cout << std::endl << "-- out of bounds on const reference --" << std::endl;
	Array<int> const &constRef = a;
	try
	{
		std::cout << constRef[100] << std::endl;
	}
	catch (std::exception const &e)
	{
		std::cout << "caught exception: " << e.what() << std::endl;
	}

	std::cout << std::endl << "-- final sizes --" << std::endl;
	std::cout << "empty.size() = " << empty.size() << std::endl;
	std::cout << "a.size() = " << a.size() << std::endl;
	std::cout << "b.size() = " << b.size() << std::endl;
	std::cout << "c.size() = " << c.size() << std::endl;

	return 0;
}
