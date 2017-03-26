#pragma once

#include "BaseEntity.h"
#include <memory>
#include <vector>


namespace ECS
{
	template<class T>
	class Group
	{
	public:
		void add(std::shared_ptr<ECS::BaseEntity> &entity);
		void remove(std::shared_ptr <ECS::BaseEntity> &entity);

		std::vector<std::shared_ptr<ECS::BaseEntity>>& getEntities();

	private:
		std::vector<std::shared_ptr<ECS::BaseEntity>> entities;
	};

	template <class T>
	void Group<T>::add(std::shared_ptr<ECS::BaseEntity>& entity)
	{
		entities.push_back(entity);
	}

	template <class T>
	void Group<T>::remove(std::shared_ptr<ECS::BaseEntity>& entity)
	{
		auto find = std::find(begin(entities), end(entities), entity);
		if (find != end(entities))
		{
			std::swap(entities.back(), *find);
			entities.pop_back();
		}
	}

	template <class T>
	std::vector<std::shared_ptr<ECS::BaseEntity>>& Group<T>::getEntities()
	{
		return entities;
	}
}