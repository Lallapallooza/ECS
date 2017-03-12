#pragma once
#include "stdafx.h"
#include "Pool.h"
#include "BaseComponent.h"

namespace ECS
{

	template<class T>
	class TPool : public Pool
	{
	public:
		static void add(T& comp) noexcept;
		static void add(std::shared_ptr<T>& comp) noexcept;
		static void remove(std::shared_ptr<T>& comp);
		static std::list<std::shared_ptr<T>>& getComponents();

		TPool();
		~TPool();
	private:
		static std::list<std::shared_ptr<T>> components;
	};

	template <class T>
	std::list<std::shared_ptr<T>> TPool<T>::components = {};

	template <class T>
	void TPool<T>::add(T& comp) noexcept
	{
		components.emplace_back(comp);
	}

	template <class T>
	void TPool<T>::add(std::shared_ptr<T>& comp) noexcept
	{
		components.push_back(comp);
	}

	template <class T>
	void TPool<T>::remove(std::shared_ptr<T>& comp)
	{
		try
		{
			components.remove(comp);
		}
		catch(std::exception &e)
		{
			std::cout << e.what() << std::endl;
		}
	}

	template <class T>
	std::list<std::shared_ptr<T>>& TPool<T>::getComponents()
	{
		return components;
	}

	template <class T>
	TPool<T>::TPool()
	{
	}

	template <class T>
	TPool<T>::~TPool()
	{
	}
}
