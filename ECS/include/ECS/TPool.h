#pragma once
#include <vector>
#include <memory>
#include <list>

namespace ECS
{

	template<class T>
	class TPool
	{
	public:
		static void add(T& comp) noexcept;
		static void add(std::shared_ptr<T>& comp) noexcept;
		static void remove(std::shared_ptr<T>& comp) noexcept;
		static std::vector<std::shared_ptr<T>>& getComponents();
		static std::vector<BaseEntity*> getEntities();

		TPool();
		~TPool();
	private:
		static std::vector<std::shared_ptr<T>> components;
	};

	template <class T>
	std::vector<std::shared_ptr<T>> TPool<T>::components = {};

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
	void TPool<T>::remove(std::shared_ptr<T>& comp) noexcept
	{
		auto find = std::find(begin(components), end(components), comp);
		if (find != end(components))
		{
			std::swap(components.back(), *find);
			components.pop_back();
		}
	}

	template <class T>
	std::vector<BaseEntity*> TPool<T>::getEntities()
	{
		std::vector<BaseEntity*> entities;
		for(const auto &comp : components)
		{
			entities.push_back(comp->entity);
		}
		return entities;
	}

	template <class T>
	std::vector<std::shared_ptr<T>>& TPool<T>::getComponents()
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
