#ifndef RIRI_HPP
# define RIRI_HPP

/*!
  @file riri.hpp
  @brief ...
  */

# include "../interface.hpp"

# include <iostream>

class fifi : public test_interface {
	public:
		void	init(void) {
			std::cout << __PRETTY_FUNCTION__ << std::endl;
		}

		void	open(void) {
			std::cout << __PRETTY_FUNCTION__ << std::endl;
		}

		void	close(void) {
			std::cout << __PRETTY_FUNCTION__ << std::endl;
		}

		virtual	~fifi() {
			std::cout << __PRETTY_FUNCTION__ << std::endl;
		}
};

extern "C" fifi	*allocator(void);
extern "C" void	deleter(fifi *o);

#endif
