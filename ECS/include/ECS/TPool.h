#pragma once
#include <vector>
#include <memory>
#include <list>
#include <functional>

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
		static void serialize();
		static void deserialize();
		static void addSerializator(std::function<void(T)> &serializator);
		static void addDeserializator(std::function<void(T)> &deserializator);
		static void removeSerializator();
		static void removeDeserializator();
		static std::function<void(T)> getDeserializator();
		static std::function<void(T)> getSerializator();
		TPool();
		~TPool();
	private:
		static std::function<void(T)> serializator;
		static std::function<void(T)> deserializator;
		static std::vector<std::shared_ptr<T>> components;
	};

	template <class T>
	std::function<void(T)> TPool<T>::serializator = nullptr;

	template <class T>
	std::function<void(T)> TPool<T>::deserializator = nullptr;

	template <class T>
	std::vector<std::shared_ptr<T>> TPool<T>::components = {};
	
	template<class T>
	void TPool<T>::deserialize()
	{
		for (auto &x : components)
		{
			deserializator(x);
		}
	}

	template<class T>
	void TPool<T>::serialize()
	{
		for(auto &x : components)
		{
			serializator(x);
		}
	}

	template<class T>
	std::function<void(T)> TPool<T>::getSerializator()
	{
		return serializator;
	}

	template<class T>
	std::function<void(T)> TPool<T>::getDeserializator()
	{
		return deserializator;
	}


	template <class T>
	void TPool<T>::removeDeserializator()
	{
		deserializator = nullptr;
	}

	template <class T>
	void TPool<T>::removeSerializator()
	{
		serializator = nullptr;
	}
	
	template <class T>
	void TPool<T>::addSerializator(std::function<void(T)> &serializator)
	{
		this->serializator = serializator;
	}

	template <class T>
	void TPool<T>::addDeserializator(std::function<void(T)> &deserializator)
	{
		this->deserializator = deserializator;
	}

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
