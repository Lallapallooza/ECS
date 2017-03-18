#pragma once
#include "BaseComponent.h"
#include "Pool.h"
#include "TPool.h"
#include <iostream>

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
		std::list<BaseEntity*> getEntities();

		static PoolManager& instance() noexcept;
		~PoolManager();
	private:
		PoolManager();
	};

	template <class ComponentType>
	void PoolManager::registerComponent(std::shared_ptr<ComponentType>& comp) noexcept
	{
		static_assert(std::is_base_of<ECS::BaseComponent, ComponentType>::value,
			"Error, delivered class is not child of ECS::BaseComponent class");
		TPool<ComponentType>::add(comp);
	}

	template <class ComponentType>
	void PoolManager::unregisterComponent(std::shared_ptr<ComponentType>& comp)
	{
		static_assert(std::is_base_of<ECS::BaseComponent, ComponentType>::value,
			"Error, delivered class is not child of ECS::BaseComponent class");
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
	std::list<BaseEntity*> PoolManager::getEntities()
	{
		std::list<BaseEntity*> to_ret;
		std::for_each(begin(TPool<ComponentType>::components), end((TPool<ComponentType>::components)), 
			[&](std::shared_ptr<ComponentType>& comp)
		{
			to_ret.push_back(comp.getEntity());
		});
		return to_ret;
	}
}
