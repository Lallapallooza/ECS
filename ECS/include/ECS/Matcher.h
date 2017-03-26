#pragma once
#include <vector>
#include <memory>
#include <unordered_set>
#include "BaseEntity.h"
#include "TPool.h"


namespace ECS
{
	class Matcher
	{
	public:
		template<class T>
		static std::vector<std::shared_ptr<ECS::BaseEntity>> findEntities();

		template<class T, class... Args>
		static std::unordered_set<std::shared_ptr<ECS::BaseEntity>> findEntities();
	private:
		template<class T>
		static void saveSame(std::unordered_set<std::shared_ptr<ECS::BaseEntity>> &all);

		template<class T, class... Args>
		static void saveSame(std::unordered_set<std::shared_ptr<ECS::BaseEntity>> &all);

	};

	template <class T>
	std::vector<std::shared_ptr<ECS::BaseEntity>> Matcher::findEntities()
	{
		std::vector<std::shared_ptr<ECS::BaseEntity>> to_ret_entitites;
		auto entities = ECS::TPool<T>::getComponents();
		for (auto &&x : entities)
		{
			to_ret_entitites.emplace_back(x->entity);
		}
		return to_ret_entitites;
	}

	template <class T, class ... Args>
	std::unordered_set<std::shared_ptr<ECS::BaseEntity>> Matcher::findEntities()
	{
		std::vector<std::shared_ptr<ECS::BaseEntity>> all_entities = findEntities<Args>();
		std::unordered_set<std::shared_ptr<ECS::BaseEntity>> all;
		for (auto &&x : all_entities)
		{
			all.insert(x);
		}
		saveSame<Args>(all);
		
		return all;
	}

	template <class T>
	void Matcher::saveSame(std::unordered_set<std::shared_ptr<ECS::BaseEntity>>& all)
	{
		std::vector<std::shared_ptr<BaseEntity>> to_erase;
		for(auto &x : all)
		{
			if(x->getComponentsByType<T>().size() == 0)
			{
				to_erase.emplace_back(x);
			}
		}
		for(auto &&x : to_erase)
		{
			all.erase(x);
		}
	}

	template <class T, class ... Args>
	void Matcher::saveSame(std::unordered_set<std::shared_ptr<ECS::BaseEntity>>& all)
	{
		saveSame<Args>(all);
	}
}
