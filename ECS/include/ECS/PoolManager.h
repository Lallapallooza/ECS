#pragma once
#include "BaseComponent.h"
#include "TPool.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

namespace ECS
{
	class PoolManager
	{
	public:
		template<class ComponentType>
		void registerComponent(std::shared_ptr<ComponentType>& comp) noexcept;
		
		template<class ComponentType>
		void unregisterComponent(std::shared_ptr<ComponentType>& comp);

		template<class ComponentType>
		std::vector<BaseEntity*> getEntities();

		static PoolManager& instance() noexcept;

		~PoolManager(){};
	private:
		PoolManager(){};
	};

	inline PoolManager& PoolManager::instance() noexcept
	{
		static PoolManager pool;
		return pool;
	}

	template <class ComponentType>
	void PoolManager::registerComponent(std::shared_ptr<ComponentType>& comp) noexcept
	{
#ifdef DEBUG
		static_assert(std::is_base_of<ECS::BaseComponent, ComponentType>::value,
			"Error, delivered class is not child of ECS::BaseComponent class");
#endif // DEBUG

		TPool<ComponentType>::add(comp);
	}

	template <class ComponentType>
	void PoolManager::unregisterComponent(std::shared_ptr<ComponentType>& comp)
	{
		
#ifdef DEBUG
		static_assert(std::is_base_of<ECS::BaseComponent, ComponentType>::value,
			"Error, delivered class is not child of ECS::BaseComponent class");
#endif // DEBUG
		try
		{
			TPool<ComponentType>::remove(comp);
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
		}
	}

	template <class ComponentType>
	std::vector<BaseEntity*> PoolManager::getEntities()
	{
		std::vector<BaseEntity*> to_ret(TPool<ComponentType>::size());
		for(const auto &x : TPool<ComponentType>::components)
		{
			to_ret.emplace_back(x);
		}
		return to_ret;
	}
}
