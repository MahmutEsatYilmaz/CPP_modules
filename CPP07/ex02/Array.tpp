#ifndef ARRAY_TPP
#define ARRAY_TPP

#include "Array.hpp"
#include <stdexcept>

template <typename T>
Array<T>::Array() : _data(new T[0]()), _size(0)
{
}

template <typename T>
Array<T>::Array(unsigned int n) : _data(new T[n]()), _size(n)
{
}

template <typename T>
Array<T>::Array(Array const &other) : _data(new T[other._size]), _size(other._size)
{
	for (unsigned int i = 0; i < _size; ++i)
		_data[i] = other._data[i];
}

template <typename T>
Array<T> &Array<T>::operator=(Array const &other)
{
	if (this == &other)
		return *this;
	T *newData = new T[other._size];
	for (unsigned int i = 0; i < other._size; ++i)
		newData[i] = other._data[i];
	delete[] _data;
	_data = newData;
	_size = other._size;
	return *this;
}

template <typename T>
Array<T>::~Array()
{
	delete[] _data;
}

template <typename T>
T &Array<T>::operator[](unsigned int idx)
{
	if (idx >= _size)
		throw std::out_of_range("Array: index out of bounds");
	return _data[idx];
}

template <typename T>
T const &Array<T>::operator[](unsigned int idx) const
{
	if (idx >= _size)
		throw std::out_of_range("Array: index out of bounds");
	return _data[idx];
}

template <typename T>
unsigned int Array<T>::size() const
{
	return _size;
}

#endif
