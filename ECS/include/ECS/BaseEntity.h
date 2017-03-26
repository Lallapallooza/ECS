#pragma once
#include <vector>
#include <unordered_map>
#include "BaseComponent.h"
#include <memory>

namespace ECS
{
	class BaseEntity
	{
	public:
		BaseEntity(){};
		std::vector<std::shared_ptr<BaseComponent>> getComponents() noexcept;
		template<class ComponentType>
		void addComponent(std::shared_ptr<ComponentType>& comp) noexcept;
		template<class ComponentType>
		void removeComponent(std::shared_ptr<ComponentType>& comp) noexcept;
		template<class ComponentType>
		std::vector<std::shared_ptr<BaseComponent>> getComponentsByType() noexcept;

		virtual ~BaseEntity(){};
	protected:
		std::unordered_map<int, std::vector<std::shared_ptr<BaseComponent>>> components;
	};

	inline std::vector<std::shared_ptr<BaseComponent>> BaseEntity::getComponents() noexcept
	{
		std::vector<std::shared_ptr<BaseComponent>> to_ret;
		for(const auto &vec : components)
		{
			for(const auto &elem : vec.second)
			{
				to_ret.push_back(elem);
			}
		}
		return to_ret;
	}

	template <class ComponentType>
	void BaseEntity::addComponent(std::shared_ptr<ComponentType>& comp) noexcept
	{
		components.push_back(comp);
		comp->entity = this;
	}

	template <class ComponentType>
	void BaseEntity::removeComponent(std::shared_ptr<ComponentType>& comp) noexcept
	{
		auto find = std::find(begin(components), end(components), comp);
		if (find != end(components))
		{
			std::swap(components.back(), find);
			components.pop_back();
			comp->entity = nullptr;
		}
	}

	template <class ComponentType>
	inline std::vector<std::shared_ptr<BaseComponent>> BaseEntity::getComponentsByType() noexcept
	{
		std::vector<std::shared_ptr<ComponentType>> to_ret;
		auto vec = components[ComponentType::id];
		for(const auto &x : vec)
		{
			to_ret.push_back(reinterpret_cast<ComponentType*>(x));
		}
		return to_ret;
	}
}
