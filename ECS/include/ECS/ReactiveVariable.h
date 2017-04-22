#pragma once
#include <functional>
#include <utility>
#include <iostream>
#include "Event.h"

namespace ECS
{
	namespace tools
	{
		template<class T, class... Args>
		class r_var
		{
		public:
			T value;
		
		public:
			T& operator=(T&& other) noexcept;
			T& operator=(T& other) noexcept;
			T& operator=(r_var<T>&& other) noexcept;
			T& operator=(r_var<T>& other) noexcept;
			T& operator+=(const T& rhs) noexcept;
			T& operator+=(const r_var<T>& rhs) noexcept;
			T& operator-=(const T& rhs) noexcept;
			T& operator-=(const r_var<T>& rhs) noexcept;

		public:
			void removeOnEnable() noexcept;
			void removeOnDisable() noexcept;
			bool removeHandle(const std::function<T(Args...)> &func) noexcept;
			bool changeCondition(const std::function<T(Args...)> &func, const  std::function<bool(const T& other)> &condition) noexcept;
			void addOnEnable(const std::function<T(Args...)> &func) noexcept;
			void addOnDisable(const std::function<T(Args...)> &func) noexcept;
			void addHandleWithCondition(const std::function<T(Args...)> &func, const  std::function<bool(const T& other)> &condition);
			void addHandle(const std::function<T(Args...)> &func);
			void raiseWithValue(Args... args) noexcept;
			void check() noexcept;
			friend std::ostream& operator<<(std::ostream& os, const r_var& st)
			{
				std::cout << st.value;
				return os;
			}

			r_var(){};
			r_var(const std::function<void()> &on_enable, const std::function<void()> &on_disable);
			~r_var(){};
		private:
			std::size_t cond_num = 0;
			std::pair<Event<void>, std::vector<std::function<bool(const T& other)>>> events;
			Event<void> onEnable;
			Event<void> onDisable;
		};



		template <class T, class ... Args>
		T& r_var<T, Args...>::operator=(T&& other) noexcept
		{
			value = std::move(other);
			check();
			return *this;
		}

		template <class T, class ... Args>
		T& r_var<T, Args...>::operator=(T& other) noexcept
		{
			value = other;
			check();
			return *this;
		}

		template <class T, class ... Args>
		T& r_var<T, Args...>::operator=(r_var<T>&& other) noexcept
		{
			value = std::move(other.value);
			check();
			return *this;
		}

		template <class T, class ... Args>
		T& r_var<T, Args...>::operator=(r_var<T>& other) noexcept
		{
			value = other.value;
			check();
			return *this;
		}

		template <class T, class ... Args>
		T& r_var<T, Args...>::operator+=(const T& rhs) noexcept
		{
			value += rhs;
			check();
			return *this;
		}

		template <class T, class ... Args>
		T& r_var<T, Args...>::operator+=(const r_var<T>& rhs) noexcept
		{
			value += rhs.value;
			check();
			return *this;
		}

		template <class T, class ... Args>
		T& r_var<T, Args...>::operator-=(const T& rhs) noexcept
		{
			value -= rhs;
			check();
			return *this;
		}

		template <class T, class ... Args>
		T& r_var<T, Args...>::operator-=(const r_var<T>& rhs) noexcept
		{
			value -= rhs.value;
			check();
			return *this;
		}

		template <class T, class ... Args>
		void r_var<T, Args...>::removeOnEnable() noexcept
		{
			onEnable.clear();
		}

		template <class T, class ... Args>
		void r_var<T, Args...>::removeOnDisable() noexcept
		{
			onDisable.clear();
		}

		template <class T, class ... Args>
		bool r_var<T, Args...>::removeHandle(const std::function<T(Args...)>& func) noexcept
		{
			bool is_erased = false;
			events.first.erase(func, is_erased);
			return is_erased;
		}

		template <class T, class ... Args>
		bool r_var<T, Args...>::changeCondition(const std::function<T(Args...)>& func, const std::function<bool(const T& other)>& condition) noexcept
		{
			std::size_t inx = events.first.find(func);
			if(inx != -1)
			{
				events.second[inx] = condition;
				return true;
			}
			return false;
		}

		template <class T, class ... Args>
		void r_var<T, Args...>::addOnEnable(const std::function<T(Args...)>& func)
		{
			onEnable += func;
		}

		template <class T, class ... Args>
		void r_var<T, Args...>::addOnDisable(const std::function<T(Args...)>& func)
		{
			onDisable += func;
		}

		template <class T, class ... Args>
		void r_var<T, Args...>::addHandleWithCondition(const std::function<T(Args...)>& func,const std::function<bool(const T& othre)>& condition)
		{
			events.first += func;
			++cond_num;
			events.second[cond_num] += condition;
		}

		template <class T, class ... Args>
		void r_var<T, Args...>::addHandle(const std::function<T(Args...)>& func)
		{
			events.first += func;
			++cond_num;
			events.second[cond_num] += [](const T& other) -> bool { return true; };
		}


		template <class T, class ... Args>
		void r_var<T, Args...>::raiseWithValue(Args... args) noexcept
		{
			for(std::size_t i = 0, size = events.size(); i < size; ++i)
			{
				if(events[i].second(args))
				{
					events[i].first[i]();
				}
			}
		}

		template <class T, class ... Args>
		r_var<T, Args...>::r_var(const std::function<void()> &on_enable,const std::function<void()> &on_disable)
		{
			onEnable += on_enable;
			onDisable += on_disable;
		}

		template <class T, class ... Args>
		void r_var<T, Args...>::check() noexcept
		{
			for (std::size_t i = 0, size = events.size(); i < size; ++i)
			{
				if (events[i].second(value))
				{
					events[i].first[i]();
				}
			}
		}

		typedef r_var<int> r_int;
		typedef r_var<double> r_double;
		typedef r_var<std::string> r_string;
		typedef r_var<bool> r_bool;
	}
}