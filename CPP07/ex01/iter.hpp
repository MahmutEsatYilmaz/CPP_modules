#ifndef ITER_HPP
#define ITER_HPP

#include <cstddef>

template <typename T, typename F>
void iter(T *array, size_t const len, F f)
{
	for (size_t i = 0; i < len; ++i)
		f(array[i]);
}

#endif
