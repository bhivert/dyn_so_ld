/*
* main.cpp
* Copyright (C) 2025 Benoit Hivert <hivert.benoit@gmail.com>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the 'Software'), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of <copyright holders>
* shall not be used in advertising or otherwise to promote the sale, use or
* other dealings in this Software without prior written authorization
* from copyright holders.
*
* Created on 2019/01/31 at 19:37:13 by Benoit Hivert <hivert.benoit@gmail.com>
* Updated on 2025/03/16 at 18:58:48 by Benoit Hivert <hivert.benoit@gmail.com>
*/

/*!
  @file main.cpp
  @brief ...
  */

#include <cstdlib>
#include <iostream>
#include <algorithm>
#include "dyn_loader.hpp"

#include "interface.hpp"

const std::vector< std::string > _libs = {
	"./riri.so",
	"./fifi.so"
};

int	main(void) {
	size_t i = (size_t)-1;
//	while (++i < 20000) {
		std::cout << "=====" << std::to_string(i) << "=====" << std::endl;
		std::cout << "[ asked to load" << std::endl;
		for (auto it = _libs.begin(); it != _libs.end(); ++it) {
			std::cout << '\t' << *it << std::endl;
		}
		std::cout << ']' << std::endl;
	while (++i < 20000) {
		dyn::loader< test_interface >	ld(_libs);

		std::vector<dyn::so<test_interface>>	interfaces({
				ld.new_object("./riri.so"),
				ld.new_object("./fifi.so"),
				ld.new_object("./riri.so"),
				ld.new_object("./fifi.so")
			});

		std::cout << "[ iter" << std::endl;
		std::for_each(interfaces.begin(), interfaces.end(), [](dyn::so< test_interface > p){
				p->init();
				p->open();
				p->close();
			});
		std::cout << ']' << std::endl;

		std::cout << "[ interface" << std::endl;
		dyn::so<test_interface>	t11 = ld.new_object("./fifi.so");
		dyn::so<test_interface>	t12 = ld.new_object("./riri.so");

		std::cout << "[ unload" << std::endl;
		ld.unload_all();
		std::cout << ']' << std::endl;

		std::cout << "[ delete fifi" << std::endl;
		t11 = t12;
		t11 = nullptr;
		t12 = nullptr;
		std::cout << ']' << std::endl;

//		dyn::so<test_interface>	t13 = ld.new_object("./riri.so");
	}
	std::cout << "[ main out" << std::endl;
	return EXIT_FAILURE;
}
