#pragma once
#include "Entity.h"
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

		std::vector<bool> serializeComponents() noexcept
		{
			const std::size_t size = serializator.size();
			std::vector<bool> bools(size);
			
			for (std::size_t i = 0; i < size; ++i)
			{
				bools[i] = serializator[i]();
			}
			return std::move(bools);
		}

		
		std::vector<bool> deserializeComponents() noexcept
		{
			const std::size_t size = deserializator.size();
			std::vector<bool> bools(size);

			for (std::size_t i = 0; i < size; ++i)
			{
				bools[i] = deserializator[i]();
			}

			return std::move(bools);
		}

		template<class ComponentType>
		void registerComponent(std::shared_ptr<ComponentType>& comp) noexcept;
		
		template<class ComponentType>
		void unregisterComponent(std::shared_ptr<ComponentType>& comp) noexcept;

		template<class ComponentType>
		std::vector<Entity*> getEntities();

		static PoolManager& instance() noexcept;

		~PoolManager(){};
	private:
		unsigned int tpool_id = 0;
		std::vector<std::function<bool()>> serializator;
		std::vector<std::function<bool()>> deserializator;
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
		TPool<ComponentType>::id = tpool_id;
		deserializator.push_back(TPool<ComponentType>::deserialize);
		serializator.push_back(TPool<ComponentType>::serialize);
		++tpool_id;

	}

	template <class ComponentType>
	void PoolManager::unregisterComponent(std::shared_ptr<ComponentType>& comp) noexcept
	{
		
#ifdef DEBUG
		static_assert(std::is_base_of<ECS::BaseComponent, ComponentType>::value,
			"Error, delivered class is not child of ECS::BaseComponent class");
#endif // DEBUG
		TPool<ComponentType>::remove(comp);
	}

	template <class ComponentType>
	std::vector<Entity*> PoolManager::getEntities()
	{
		std::vector<BaseEntity*> to_ret(TPool<ComponentType>::size());
		for(const auto &x : TPool<ComponentType>::components)
		{
			to_ret.emplace_back(x);
		}
		return to_ret;
	}
}
