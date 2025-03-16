/*!
  @file riri.cpp
  @brief ...
  */

#include "riri.hpp"

fifi	*allocator(void) {
	return new fifi();
}

void	deleter(fifi *o) {
	delete o;
}
