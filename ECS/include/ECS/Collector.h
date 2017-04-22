#pragma once	
#include <memory>
#include <vector>
#include "PoolManager.h"

namespace ECS
{
	namespace tools
	{
		class Collector
		{
		public:	
			template<class Type>
			static std::shared_ptr<Type> instantiate();

			template<class Type>
			static void destroy(std::shared_ptr<Type> &comp);
		private:
			static std::vector<unsigned int> components_ids;
		};

		template <class Type>
		std::shared_ptr<Type> Collector::instantiate()
		{
#ifndef DEBUG
			static_assert(std::is_base_of<ECS::BaseComponent, Type>::value,
				"Error, delivered class is not child of ECS::BaseComponent class");
#endif // DEBUG
			std::shared_ptr<Type> comp_ptr = std::make_shared<Type>();
			PoolManager::instance().registerComponent<Type>(comp_ptr);
			if (Type::id == 0)
			{
				Type::id = components_ids.size();
				components_ids.push_back(Type::id);
			}
			return comp_ptr;
		}

		template <class Type>
		void Collector::destroy(std::shared_ptr<Type>& comp)
		{
#ifdef DEBUG
			static_assert(std::is_base_of<ECS::BaseComponent, Type>::value,
				"Error, delivered class is not child of ECS::BaseComponent class");
#endif // DEBUG

			PoolManager::instance().unregisterComponent<Type>(comp);
			auto entity = comp->entity;
			if (entity)
			{
				entity->removeComponent(comp);
			}
			comp.reset();
		}
	}
}