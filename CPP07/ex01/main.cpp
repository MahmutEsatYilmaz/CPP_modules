#include <iostream>
#include <string>
#include "iter.hpp"

template <typename T>
void print(T const &elem)
{
	std::cout << elem << " ";
}

void doubleInt(int &elem)
{
	elem *= 2;
}

int main(void)
{
	int arr[5] = {1, 2, 3, 4, 5};

	std::cout << "-- non-const int array, print --" << std::endl;
	iter(arr, 5, print<int>);
	std::cout << std::endl;

	std::cout << "-- non-const int array, mutate (double each element) --" << std::endl;
	iter(arr, 5, doubleInt);
	iter(arr, 5, print<int>);
	std::cout << std::endl;

	int const carr[5] = {10, 20, 30, 40, 50};

	std::cout << "-- const int array, print (proves const T works) --" << std::endl;
	iter(carr, 5, print<int>);
	std::cout << std::endl;

	std::string strArr[3] = {"foo", "bar", "baz"};

	std::cout << "-- non-const std::string array, print --" << std::endl;
	iter(strArr, 3, print<std::string>);
	std::cout << std::endl;

	return 0;
}
