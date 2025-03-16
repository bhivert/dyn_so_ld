/*
* interface.hpp is part of a project.
* Copyright (C) 2019  <hivert.benoit@gmail.com>
*
* This project is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This project is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>
*
* Created on 2019/02/03 at 23:05:14 by  <hivert.benoit@gmail.com>
* Updated on 2019/02/06 at 20:47:02 by  <hivert.benoit@gmail.com>
*/

#ifndef INTERFACE_HPP
# define INTERFACE_HPP

/*!
  @file interface.hpp
  @brief ...
  */

class test_interface {
	public:
		virtual void	init(void) = 0;
		virtual void	open(void) = 0;
		virtual void	close(void) = 0;
};

#endif
