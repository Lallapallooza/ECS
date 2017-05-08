#pragma once
#include <vector>
#include <functional>
#include <utility>

namespace ECS
{
	namespace tools
	{
		template<class T, class... U>
		class Event
		{
		public:
			Event &operator+=(const std::function<T(U...)>& func) noexcept;
			Event &operator-=(const std::function<T(U...)>& func) noexcept;
			Event &erase(const std::function<T(U...)>& func, bool &is_erased) noexcept;
			void clear() noexcept;
			std::size_t size() noexcept;
			const std::function<T(U...)>& operator[](int inx) const noexcept;
			std::size_t find(const std::function<T(U...)> &func) const noexcept;
			std::vector<T> notify(U... args);
		private:
			void operator=(const Event &other) = delete;
			std::vector<std::function<T(U...)>> _functions;

			static std::size_t getAddress(std::function<T(U ...)> &f)
			{
				return std::size_t(std::addressof(f));
			}
		};

		template<class T, class... U>
		void Event<T, U...>::clear() noexcept
		{
			_functions.clear();
		}

		template <class T, class ... U>
		std::size_t Event<T, U...>::size() noexcept
		{
			return _functions.size();
		}

		template <class T, class ... U>
		const std::function<T(U...)>& Event<T, U...>::operator[](int inx) const noexcept
		{
			return _functions[inx];
		}

		template <class T, class ... U>
		std::size_t Event<T, U...>::find(const std::function<T(U...)>& func) const noexcept
		{
			std::size_t size = _functions.size();
			for (std::size_t i = 0; i < size; ++i)
			{
				if (getAddress(_functions[i]) == getAddress(func))
				{
					return i;
				}
			}
			return -1;
		}

		template<class T, class... U>
		std::vector<T> Event<T, U...>::notify(U...args)
		{
			std::size_t size = _functions.size();

			std::vector<T> to_ret(size);
			for (std::size_t i = 0; i < size; ++i)
			{
				to_ret[i] = std::move(_functions[i](args...));
			}
			return std::move(to_ret);
		}


		template<class T, class... U>
		Event<T, U...>& Event<T, U...>::operator+=(const std::function<T(U...)>& func) noexcept
		{
			this->_functions.push_back(func);
			return *this;
		}

		template<class T, class... U>
		Event<T, U...>& Event<T, U...>::operator-=(const std::function<T(U...)>& func) noexcept
		{
			std::size_t size = _functions.size();

			for (std::size_t i = 0; i < size; ++i)
			{
				if (getAddress(_functions[i]) == getAddress(func))
				{
					std::swap(_functions.back(), _functions[i]);
					_functions.pop_back();
					break;
				}
			}
			return *this;
		}

		template <class T, class ... U>
		Event<T, U...>& Event<T, U...>::erase(const std::function<T(U...)>& func, bool& is_erased) noexcept
		{
			std::size_t size = _functions.size();
			is_erased = false;
			for (std::size_t i = 0; i < size; ++i)
			{
				if (getAddress(_functions[i]) == getAddress(func))
				{
					std::swap(_functions.back(), _functions[i]);
					_functions.pop_back();
					is_erased = true;
					break;
				}
			}
			return *this;
		}



		template<class... U>
		class Event<void, U...>
		{
		public:
			Event &operator+=(const std::function<void(U...)>& func) noexcept;
			Event &operator-=(const std::function<void(U...)>& func) noexcept;
			Event &erase(const std::function<void(U...)>& func, bool &is_erased) noexcept;
			void clear() noexcept;
			std::size_t size() noexcept;
			const std::function<void(U...)>& operator[](int inx) const noexcept;
			std::size_t find(const std::function<void(U...)> &func) const noexcept;
			void notify(U... args);
		private:
			void operator=(const Event &other) = delete;
			std::vector<std::function<void(U...)>> _functions;

			static std::size_t getAddress(std::function<void(U ...)> f)
			{
				return std::size_t(std::addressof(f));
			}
		};

		template<class... U>
		void Event<void, U...>::clear() noexcept
		{
			_functions.clear();
		}

		template<class ... U>
		std::size_t Event<void, U...>::size() noexcept
		{
			return _functions.size();
		}

		template<class ... U>
		const std::function<void(U...)>& Event<void, U...>::operator[](int inx) const noexcept
		{
			return _functions[inx];
		}

		template<class ... U>
		std::size_t Event<void, U...>::find(const std::function<void(U...)>& func) const noexcept
		{
			std::size_t size = _functions.size();
			for (std::size_t i = 0; i < size; ++i)
			{
				if (getAddress(_functions[i]) == getAddress(func))
				{
					return i;
				}
			}
			return -1;
		}

		template<class... U>
		void Event<void, U...>::notify(U...args)
		{
			std::size_t size = _functions.size();

			for (std::size_t i = 0; i < size; ++i)
			{
				_functions[i](args...);
			}
		}


		template<class... U>
		Event<void, U...>& Event<void, U...>::operator+=(const std::function<void(U...)>& func) noexcept
		{
			this->_functions.push_back(func);
			return *this;
		}

		template<class... U>
		Event<void, U...>& Event<void, U...>::operator-=(const std::function<void(U...)>& func) noexcept
		{
			std::size_t size = _functions.size();

			for (std::size_t i = 0; i < size; ++i)
			{
				if (getAddress(_functions[i]) == getAddress(func))
				{
					std::swap(_functions.back(), _functions[i]);
					_functions.pop_back();
					break;
				}
			}
			return *this;
		}

		template<class ... U>
		Event<void, U...>& Event<void, U...>::erase(const std::function<void(U...)>& func, bool& is_erased) noexcept
		{
			std::size_t size = _functions.size();
			is_erased = false;
			for (std::size_t i = 0; i < size; ++i)
			{
				if (getAddress(_functions[i]) == getAddress(func))
				{
					std::swap(_functions.back(), _functions[i]);
					_functions.pop_back();
					is_erased = true;
					break;
				}
			}
			return *this;
		}


	}
	extern tools::Event<void> preUpdate;
	extern tools::Event<void> Update;
	extern tools::Event<void> lateUpdate;
}
