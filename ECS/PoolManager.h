#pragma once
#include "BaseComponent.h"
#include "Pool.h"
#include "TPool.h"

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
		void unregisterComponent(ComponentType& comp);
		
		static PoolManager& instance() noexcept;
		std::unordered_map<std::shared_ptr<BaseComponent>, std::shared_ptr<Pool>> registers;
		~PoolManager();
	private:
		PoolManager();
	};

	template <class ComponentType>
	void PoolManager::registerComponent(std::shared_ptr<ComponentType>& comp) noexcept
	{
		static_assert(std::is_base_of<ECS::BaseComponent, ComponentType>::value);
		auto got = registers.find(comp);
		if (got == end(registers))
		{
			auto &pool = registers[comp];
			pool = std::make_shared<TPool<ComponentType>>();
			std::dynamic_pointer_cast<TPool<ComponentType>>(pool)->add(comp);
		}
		else
		{
			std::dynamic_pointer_cast<TPool<ComponentType>>(registers[comp])->add(comp);
		}

	}

	template <class ComponentType>
	void PoolManager::unregisterComponent(std::shared_ptr<ComponentType>& comp)
	{
		static_assert(std::is_base_of<ECS::BaseComponent, ComponentType>::value);
		auto got = registers.find(comp);
		try
		{
			if (got == end(registers))
			{
				throw std::runtime_error("Components not found");
			}
			else
			{
				std::dynamic_pointer_cast<TPool<ComponentType>>(&registers[comp])->remove(comp);
			}
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
		}
	}

	template <class ComponentType>
	void PoolManager::unregisterComponent(ComponentType& comp)
	{
		static_assert(std::is_base_of<ECS::BaseComponent, ComponentType>::value);
		auto got = registers.find(comp);
		try
		{
			if (got == end(registers))
			{
				throw std::runtime_error("Components not found");
			}
			else
			{
				std::dynamic_pointer_cast<TPool<ComponentType>>(registers[comp])->remove(comp);
			}
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
		}
	}

}
