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
		std::vector<std::shared_ptr<ComponentType>> getComponentsByType() noexcept;
		template<class ComponentType>
		std::shared_ptr<ComponentType> getComponentByType() noexcept;

		virtual ~BaseEntity(){};
	protected:
		std::unordered_map<int, std::vector<std::shared_ptr<BaseComponent>>> components;
	};

	template<class ComponentType>
	inline std::shared_ptr<ComponentType> BaseEntity::getComponentByType() noexcept
	{
		auto it = components.find(ComponentType::id);
		if (it != components.end())
		{
			auto vec = components[ComponentType::id];
			if (vec.size() != 0)
				return std::static_pointer_cast<ComponentType>(components[ComponentType::id][0]);
			else
				return nullptr;
		}
		return nullptr;
	}

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
		components[comp->id].push_back(comp);
		comp->entity = this;
	}

	template <class ComponentType>
	void BaseEntity::removeComponent(std::shared_ptr<ComponentType>& comp) noexcept
	{
		auto &vec = components[comp->id];
		auto find = std::find(begin(vec), end(vec), comp->id);
		if (find != end(vec))
		{
			std::swap(vec.back(), find);
			vec.pop_back();
			comp->entity = nullptr;
		}
	}

	template <class ComponentType>
	inline std::vector<std::shared_ptr<ComponentType>> BaseEntity::getComponentsByType() noexcept
	{
		std::vector<std::shared_ptr<ComponentType>> to_ret;
		auto vec = components[ComponentType::id];
		for(const auto &x : vec)
		{
			to_ret.push_back(std::static_pointer_cast<ComponentType>(x));
		}
		return to_ret;
	}
}
