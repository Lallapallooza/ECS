#pragma once

#include "Entity.h"
#include "conststr.h"
#include <memory>
#include <vector>



namespace ECS
{

	template<std::size_t N>
	class Group
	{
	public:
		static void add(std::shared_ptr<Entity> &entity);
		static void remove(std::shared_ptr<Entity> &entity);

		static std::vector<std::shared_ptr<Entity>>& getEntities();

	private:
		static std::vector<std::shared_ptr<Entity>> entities;
	};

	template<unsigned int N>
	std::vector<std::shared_ptr<Entity>> Group<N>::entities = {};

	template<unsigned int N>
	void Group<N>::add(std::shared_ptr<Entity>& entity)
	{
		entities.push_back(std::static_pointer_cast<Entity>(entity));
	}

	template<unsigned int N>
	void Group<N>::remove(std::shared_ptr<Entity>& entity)
	{
		auto find = std::find(begin(entities), end(entities), std::static_pointer_cast<Entity>(entity));
		if (find != end(entities))
		{
			std::swap(entities.back(), *find);
			entities.pop_back();
		}
	}

	template<unsigned int N>
	std::vector<std::shared_ptr<Entity>>& Group<N>::getEntities()
	{
		return entities;
	}
}