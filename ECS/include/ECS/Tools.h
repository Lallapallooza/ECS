#pragma once
#include "PoolManager.h"
#include "TPool.h"
#include <functional>
#include <vector>

namespace ECS
{
	namespace tools
	{
		class percent
		{
		public:
			int percent_num;
			explicit percent(int num);
			~percent();
		};
		inline percent::percent(int num) : percent_num(num) {}
		inline percent::~percent() {}


		class Event
		{
		public:
			Event &operator+=(const std::function<void()>& func);
			Event &operator-=(const std::function<void()>& func);
			void operator()();
		private:
			void operator=(const Event &other) = delete;
			std::vector<std::function<void()>> _functions;
			template<typename T, typename... U>
			size_t getAddress(std::function<T(U...)> f)
			{
				typedef T(fnType)(U...);
				fnType **fnPointer = f.template target<fnType*>();
				return size_t(*fnPointer);
			}
		};

		inline Event& Event::operator+=(const std::function<void()>& func)
		{
			this->_functions.push_back(func);
			return *this;
		}

		inline Event& Event::operator-=(const std::function<void()>& func)
		{
			int size = _functions.size();

			for(int i = 0; i < size; ++i)
			{
				if(getAddress(_functions[i]) == getAddress(func))
				{
					std::swap(_functions.back(), _functions[i]);
					_functions.pop_back();
				}
			}
			return *this;
		}

		inline void Event::operator()()
		{
			int size = _functions.size();

			for (int i = 0; i < size; ++i)
			{
				_functions[i]();
			}
		}
		class Collector
		{
		public:
			template<class Type>
			static std::shared_ptr<Type> instantiate();

			template<class Type>
			static void destroy(std::shared_ptr<Type> &comp);
		private:
			static std::vector<int> ids;
		};


		template <class Type>
		std::shared_ptr<Type> Collector::instantiate()
		{
			static_assert(std::is_base_of<ECS::BaseComponent, Type>::value,
				"Error, delivered class is not child of ECS::BaseComponent class");
			std::shared_ptr<Type> comp_ptr = std::make_shared<Type>();
			PoolManager::instance().registerComponent<Type>(comp_ptr);
			if(Type::id == 0)
			{
				Type::id = ids.size();
				ids.push_back(Type::id);
			}
			return comp_ptr;
		}

		template <class Type>
		void Collector::destroy(std::shared_ptr<Type>& comp)
		{
			static_assert(std::is_base_of<ECS::BaseComponent, Type>::value,
				"Error, delivered class is not child of ECS::BaseComponent class"); 
			PoolManager::instance().unregisterComponent<Type>(comp);
			auto entity = comp->entity;
			if(entity)
			{
				entity->removeComponent(comp);
			}
			comp.reset();
		}
	}

	extern tools::Event preUpdate;
	extern tools::Event Update;
	extern tools::Event lateUpdate;
}


template<class T>
T operator-(const T &lhs, const ECS::tools::percent &rhs)
{
	return lhs - (lhs * rhs.percent_num / 100);
}

template<class T>
T operator+(const T &lhs, const ECS::tools::percent &rhs)
{
	return lhs + (lhs * rhs.percent_num / 100);
}

template<class T>
T operator-=(const T &lhs, const ECS::tools::percent &rhs)
{
	return lhs - (lhs * rhs.percent_num / 100);
}

template<class T>
T operator+=(const T &lhs, const ECS::tools::percent &rhs)
{
	return lhs + (lhs * rhs.percent_num / 100);
}
