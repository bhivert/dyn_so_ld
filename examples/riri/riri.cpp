/*!
  @file riri.cpp
  @brief ...
  */

#include "riri.hpp"

riri	*allocator(void) {
	return new riri();
}

void	deleter(riri *o) {
	delete o;
}
