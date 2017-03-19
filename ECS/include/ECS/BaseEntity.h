#pragma once
#include <vector>
#include "BaseComponent.h"
#include <memory>

namespace ECS
{
	class BaseEntity
	{
	public:
		BaseEntity(){};
		std::vector<std::shared_ptr<BaseComponent>>& getComponents() noexcept;
		template<class ComponentType>
		void addComponent(std::shared_ptr<ComponentType>& comp) noexcept;
		template<class ComponentType>
		void removeComponent(std::shared_ptr<ComponentType>& comp) noexcept;
		virtual ~BaseEntity(){};
	protected:
		std::vector<std::shared_ptr<BaseComponent>> components;
	};

	inline std::vector<std::shared_ptr<BaseComponent>>& BaseEntity::getComponents() noexcept
	{
		return components;
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
}
