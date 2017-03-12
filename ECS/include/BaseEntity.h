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
		void addComponent(std::shared_ptr<ComponentType>& comp);
		template<class ComponentType>
		void removeComponent(std::shared_ptr<ComponentType>& comp);
		virtual ~BaseEntity();
	protected:
		std::list<std::shared_ptr<BaseComponent>> components;
	};

	template <class ComponentType>
	void BaseEntity::addComponent(std::shared_ptr<ComponentType>& comp)
	{
		components.push_back(comp);
	}

	template <class ComponentType>
	void BaseEntity::removeComponent(std::shared_ptr<ComponentType>& comp)
	{
		components.remove(comp);
	}
}
