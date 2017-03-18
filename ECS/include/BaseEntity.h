#pragma once
#include "BaseComponent.h"
namespace ECS
{
	class BaseEntity
	{
	public:
		BaseEntity();
		std::list<std::shared_ptr<BaseComponent>>& getComponents();
		template<class ComponentType>
		void addComponent(std::shared_ptr<ComponentType>& comp) noexcept;
		template<class ComponentType>
		void removeComponent(std::shared_ptr<ComponentType>& comp) noexcept;
		virtual ~BaseEntity();
	protected:
		std::list<std::shared_ptr<BaseComponent>> components;
	};

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
			components.remove(comp);
			comp->enitity = nullptr;
		}
	}
}
