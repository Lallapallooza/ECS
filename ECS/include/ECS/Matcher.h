#pragma once
#include <vector>
#include <memory>
#include <cmath>
#include <initializer_list>
#include <unordered_set>
#include "Entity.h"
#include "TPool.h"


namespace ECS
{
	class Matcher
	{
	public:
		template<class T, class B, class... Args>
		static std::unordered_set<ECS::Entity*> matchNew();

		template<class T, class B, class... Args>
		static std::unordered_set<ECS::Entity*> match();

	private:
		template<class T, class B, class... Args>
		static void saveSame(std::unordered_set<ECS::Entity*> &all, std::vector<std::unordered_set<ECS::Entity*>::iterator> &to_erase);

		template<class T, class B, class... Args>
		static void findEntities(std::unordered_set<ECS::Entity*> &all, std::vector<ECS::Entity*> &entities, std::vector<int> &ids);

		template<class T, class B, class... Args>
		static std::vector<int> getIds();

		template<class T, class B, class... Args>
		static void getId(std::vector<int> &ids);

		template<class T>
		static void getId(std::vector<int> &ids);

		template<class T>
		static void saveSame(std::unordered_set<ECS::Entity*> &all, std::vector<std::unordered_set<ECS::Entity*>::iterator> &to_erase);

		template<class T>
		static void findEntities(std::unordered_set<ECS::Entity*> &all, std::vector<ECS::Entity*> &entities, std::vector<int> &ids);

		static int calculateId(std::vector<int> &ids);

		static std::unordered_map<int, std::unordered_set<ECS::Entity*>> saved_matches;
	};

	std::unordered_map<int, std::unordered_set<ECS::Entity*>> Matcher::saved_matches = {};

	int Matcher::calculateId(std::vector<int> &ids)
	{
		auto getNumberOfDigits = [](int i) -> int
		{
			return i > 0 ? (int)log10((double)i) + 1 : 1;
		};
		int unique_id = ids[0];
		for (int i = 1, size = ids.size(); i < size; ++i)
		{
			int digits = getNumberOfDigits(ids[i]);
			while (digits > 0)
			{
				unique_id *= 10;
				--digits;
			}
			unique_id += ids[i];
		}
		return unique_id;
	}


	template<class T, class B, class... Args>
	std::vector<int> Matcher::getIds()
	{
		std::vector<int> ids;
		getId<T, B, Args...>(ids);
		return ids;
	}

	template<class T>
	void Matcher::getId(std::vector<int> &ids)
	{
		ids.push_back(T::id);
	}

	template<class T, class B, class... Args>
	void Matcher::getId(std::vector<int> &ids)
	{
		ids.push_back(T::id);
		getId<B, Args...>(ids);
	}
	
	template<class T, class B, class... Args>
	std::unordered_set<ECS::Entity*> Matcher::match()
	{
		std::vector<int> ids = getIds<T, B, Args...>();
		std::sort(begin(ids), end(ids));
		int uniq = calculateId(ids);
		auto find = saved_matches.find(uniq);
		if (find != saved_matches.end())
		{
			return find->second;
		}
		else
			return {};
	}

	template<class T, class B, class... Args>
	void Matcher::saveSame(std::unordered_set<ECS::Entity*> &all, std::vector<std::unordered_set<ECS::Entity*>::iterator> &to_erase)
	{
		for (auto it = all.begin(); it != all.end(); ++it)
		{
			if ((*it)->getComponentByType<T>() == nullptr)
			{
				to_erase.push_back(it);
			}

		}

		for(const auto &x : to_erase)
		{
			all.erase(x);
		}
		saveSame<B, Args...>(all, to_erase);
	}

	template<class T, class B, class... Args>
	std::unordered_set<ECS::Entity*> Matcher::matchNew()
	{
		std::vector<Entity*> entities(100);
		std::unordered_set<ECS::Entity*> all;
		std::vector<std::unordered_set<ECS::Entity*>::iterator> to_erase;
		
		std::vector<int> ids;

		findEntities<T, B, Args...>(all, entities, ids);
		saveSame<T, B, Args...>(all, to_erase);
		if (all.size() != 0)
		{
			std::sort(begin(ids), end(ids));
			saved_matches[calculateId(ids)] = all;
		}
		return all;
	}

	
	template <class T, class B, class ... Args>
	void Matcher::findEntities(std::unordered_set<ECS::Entity*> &all, std::vector<ECS::Entity*> &entities, std::vector<int> &ids)
	{
		entities = ECS::TPool<T>::getEntities();
		ids.push_back(T::id);
		for (auto &&x : entities)
		{
			if (x != nullptr)
			{
				all.insert(std::move(x));
			}
		}
		findEntities<B, Args...>(all, entities, ids);
	}


	template<class T>
	void Matcher::findEntities(std::unordered_set<ECS::Entity*> &all, std::vector<ECS::Entity*> &entities, std::vector<int> &ids)
	{
		entities.clear();
		ids.push_back(T::id);
		entities = ECS::TPool<T>::getEntities();
		for (auto &&x : entities)
		{
			if (x != nullptr)
			{
				all.insert(std::move(x));
			}
		}
	}

	template <class T>
	void Matcher::saveSame(std::unordered_set<ECS::Entity*> &all, std::vector<std::unordered_set<ECS::Entity*>::iterator> &to_erase)
	{
		to_erase.clear();
		for (auto it = all.begin(); it != all.end(); ++it)
		{
			if ((*it)->getComponentByType<T>() == nullptr)
			{
				to_erase.push_back(it);
			}
		}

		for (const auto &x : to_erase)
		{
			all.erase(x);
		}
	}
}
