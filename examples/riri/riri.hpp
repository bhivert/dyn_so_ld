#ifndef RIRI_HPP
# define RIRI_HPP

/*!
  @file riri.hpp
  @brief ...
  */

# include "../interface.hpp"

# include <iostream>

class riri : public test_interface {
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

		virtual	~riri() {
			std::cout << __PRETTY_FUNCTION__ << std::endl;
		}
};

extern "C" riri	*allocator(void);
extern "C" void	deleter(riri *o);

#endif
