#ifndef ARRAY_HPP
#define ARRAY_HPP

template <typename T>
class Array
{
public:
	Array();
	Array(unsigned int n);
	Array(Array const &other);
	Array &operator=(Array const &other);
	~Array();

	T &operator[](unsigned int idx);
	T const &operator[](unsigned int idx) const;

	unsigned int size() const;

private:
	T *_data;
	unsigned int _size;
};

#include "Array.tpp"

#endif
