/*
* dyn_loader.hpp
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
* Created on 2019/01/31 at 19:37:39 by Benoit Hivert <hivert.benoit@gmail.com>
* Updated on 2025/03/16 at 19:37:35 by Benoit Hivert <hivert.benoit@gmail.com>
*/

#ifndef __DYN_LOADER_HPP__
# define __DYN_LOADER_HPP__

/*!
@file dyn_loader.hpp
@brief ...
*/

# include <dlfcn.h>
# include <unordered_map>
# include <vector>
# include <memory>
# include <mutex>

# include <ostream>
# include <string>

namespace dyn {

	enum class oflag : int {
		LAZY		= RTLD_LAZY,
		NOW			= RTLD_NOW,
		GLOBAL		= RTLD_GLOBAL,
		LOCAL		= RTLD_LOCAL,
		NODELETE	= RTLD_NODELETE,
		NOLOAD		= RTLD_NOLOAD,
		DEEPBIND	= RTLD_DEEPBIND
	};

	oflag	operator|(const oflag &lhs, const oflag &rhs) {
		return static_cast<oflag>(static_cast<int>(lhs) | static_cast<int>(rhs));
	};

	template <class I>
	class shared_object;

	template<typename I>
	using so=shared_object<I>;

	template <class I>
	class loader {
		public:
			loader(const std::string &filename, const oflag &flags = oflag::LAZY) {
				this->load(filename, flags);
			};

			loader(Lmid_t lmid, const std::string &filename, const oflag &flags = oflag::LAZY) {
				this->load(lmid, filename, flags);
			};

			loader(const std::vector<std::string> &filenames, const oflag &flags = oflag::LAZY) {
				this->load(filenames, flags);
			};

			loader(Lmid_t lmid, const std::vector<std::string> &filenames, const oflag &flags = oflag::LAZY) {
				this->load(lmid, filenames, flags);
			};

			virtual	~loader(void) {};

			void	load(const std::string &filename, const oflag &flags = oflag::LAZY) {
				std::lock_guard<std::recursive_mutex> lck(loader<I>::_so_map_lock);
				if (loader<I>::_so_map.find(filename) != loader<I>::_so_map.end()) {
					return ;
				}

				void	*handler = dlopen(filename.c_str(), static_cast<int>(flags));
				if (!handler) {
					throw std::logic_error(std::string("dyn::loader: ") + dlerror());
				}

				loader<I>::_so_map[filename] = {handler, [](so_handler_t::element_type *handler){
						dlclose(const_cast<void *>(handler));
					}};;
			};

			void	load(Lmid_t lmid, const std::string &filename, const oflag &flags = oflag::LAZY) {
				std::lock_guard<std::recursive_mutex> lck(loader<I>::_so_map_lock);
				if (loader<I>::_so_map.find(filename) != loader<I>::_so_map.end()) {
					return ;
				}

				void	*handler = dlmopen(lmid, filename.c_str(), static_cast<int>(flags));
				if (!handler) {
					throw std::logic_error(std::string("dyn::loader: ") + dlerror());
				}

				loader<I>::_so_map[filename] = {handler, [](so_handler_t::element_type *handler){
						dlclose(const_cast<void *>(handler));
					}};;
			};

			void	load(const std::vector<std::string> &filenames, const oflag &flags = oflag::LAZY) {
				for (auto it : filenames) {
					this->load(it, flags);
				}
			};

			void	load(Lmid_t lmid, const std::vector<std::string> &filenames, const oflag &flags = oflag::LAZY) {
				for (auto it : filenames) {
					this->load(lmid, it, flags);
				}
			};

			void	unload(const std::string &filename) {
				std::lock_guard<std::recursive_mutex> lck(loader<I>::_so_map_lock);
				loader<I>::_so_map.erase(filename);
			};

			void	unload(const std::vector<std::string> &filenames) {
				std::lock_guard<std::recursive_mutex> lck(loader<I>::_so_map_lock);
				for (auto it : filenames) {
					loader<I>::_so_map.erase(it);
				}
			};

			void	unload_all(void) {
				std::lock_guard<std::recursive_mutex> lck(loader<I>::_so_map_lock);
				loader<I>::_so_map.clear();
			};

			shared_object<I>	new_object(const std::string &filename) {
				std::lock_guard<std::recursive_mutex> lck(loader<I>::_so_map_lock);
				loader<I>::so_map_t::iterator	it;
				if ((it = loader<I>::_so_map.find(filename)) == loader<I>::_so_map.end()) {
					throw std::logic_error(std::string("dyn::loader: '") + filename + "' shared object not loaded.");
				}

				typename dyn::shared_object<I>::allocator_t	allocator = (typename dyn::shared_object<I>::allocator_t)dlsym(const_cast<void *>(it->second.get()), "allocator");
				if (!allocator) {
					throw std::logic_error(std::string("dyn::loader: ") + dlerror());
				}

				typename dyn::shared_object<I>::deleter_t	deleter = (typename dyn::shared_object<I>::deleter_t)dlsym(const_cast<void *>(it->second.get()), "deleter");
				if (!deleter) {
					throw std::logic_error(std::string("dyn::loader: ") + dlerror());
				}

				return shared_object<I>(filename, it->second, typename shared_object<I>::object_t(allocator(), [deleter](typename shared_object<I>::object_t::element_type *obj){ deleter(const_cast<I *>(obj)); }));
			};

		protected:
		private:
			loader(const loader &src) = delete;
			loader(loader &&src) = delete;

			loader &	operator=(const loader &src) = delete;
			loader &	operator=(loader &&src) = delete;

			typedef std::shared_ptr<const void>						so_handler_t;
			typedef std::unordered_map<std::string, so_handler_t>	so_map_t;

			static std::recursive_mutex								_so_map_lock;
			static so_map_t											_so_map;

			friend shared_object<I>;
	};

	template <class I>
	class shared_object {
		public:
			typedef I *					(*allocator_t)(void);
			typedef void				(*deleter_t)(I *);

			typedef std::shared_ptr<I>	object_t;

			shared_object(void) : _filename("nullptr"), _handler(nullptr), _obj(nullptr) {
			};

			shared_object(const shared_object &src) : _filename(src._filename), _handler(src._handler), _obj(src._obj) {
			};

			shared_object(shared_object &&src) : _filename(std::move(src._filename)), _handler(std::move(src._handler)), _obj(std::move(src._obj)) {
			};

			virtual	~shared_object(void) {};

			shared_object &	operator=(std::nullptr_t) {
				*const_cast<std::string *>(&this->_filename) = "nullptr";
				this->_handler = nullptr;
				this->_obj = nullptr;
				return *this;
			};

			shared_object &	operator=(const shared_object &rhs) {
				*const_cast<std::string *>(&this->_filename) = rhs._filename;
				this->_handler = rhs._handler;
				this->_obj = rhs._obj;
				return *this;
			};

			shared_object &	operator=(shared_object &&rhs) {
				this->_filename = std::move(rhs._filename);
				this->_handler = std::move(rhs._handler);
				this->_obj = std::move(rhs._obj);
				return *this;
			};

			operator bool(void) {
				return this->_obj.operator bool();
			};

			I *	operator->(void) {
				if (!this->operator bool()) {
					throw std::logic_error("dyn::shared_object: nullptr");
				}
				{
					std::lock_guard<std::recursive_mutex> lck(loader<I>::_so_map_lock);
					if (loader<I>::_so_map.find(this->_filename) == loader<I>::_so_map.end()) {
						throw std::logic_error(std::string("dyn::shared_object: ") + this->_filename + ": unloaded.");
					}
				}
				return this->_obj.operator->();
			};

			I &	operator*(void) {
				return *this->operator->();
			};

		protected:
		private:
			shared_object(const std::string &filename, typename loader<I>::so_handler_t &handler, const object_t &obj)
				: _filename(filename), _handler(handler), _obj(obj) {
			};

			std::string							_filename;
			typename loader<I>::so_handler_t	_handler;
			object_t							_obj;

			friend loader<I>;
	};

	template <class I>
	std::recursive_mutex loader<I>::_so_map_lock;

	template <class I>
	typename loader<I>::so_map_t loader<I>::_so_map;
};

#endif
