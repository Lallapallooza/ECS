#pragma once
#include "Entity.h"
#include <vector>
#include <memory>
#include <functional>

namespace ECS
{

	template<class T>
	class TPool
	{
	public:
		static unsigned int id;

		static void add(std::shared_ptr<T>& comp) noexcept;
		static bool remove(std::shared_ptr<T>& comp) noexcept;

		static std::vector<std::shared_ptr<T>>& getComponents() noexcept;
		static std::vector<ECS::Entity*> getEntities() noexcept;

		static std::size_t size() noexcept;

		static void addPathForDeserializer(const std::string &path) noexcept;
		static void addPathForDeserializer(const std::vector<std::string> &paths) noexcept;

		static bool serialize() noexcept;
		static bool deserialize() noexcept;
		
		static void addSerializator(std::function<void(std::shared_ptr<T> &comp)> &serializator) noexcept;
		static void addDeserializator(std::function<std::shared_ptr<T>(const std::string &path)> &deserializator) noexcept;

		static void removeSerializator() noexcept;
		static void removeDeserializator() noexcept;

		static std::function<std::shared_ptr<T>(const std::string &path)> getDeserializator() noexcept;
		static std::function<void(std::shared_ptr<T> &comp)> getSerializator() noexcept;
	private:
		
		static std::vector<std::string> paths;
		static std::function<void(std::shared_ptr<T> &comp)> serializator;
		static std::function<std::shared_ptr<T>(const std::string &path)> deserializator;
		static std::vector<std::shared_ptr<T>> components;
	};

	template<class T>
	std::vector<std::string> TPool<T>::paths = {};


	template<class T>
	int TPool<T>::id = {};

	template <class T>
	std::size_t TPool<T>::size() noexcept
	{
		return components.size();
	}

	template <class T>
	void TPool<T>::addPathForDeserializer(const std::string& path) noexcept
	{
		paths.emplace_back(path);
	}

	template <class T>
	void TPool<T>::addPathForDeserializer(const std::vector<std::string>& paths) noexcept
	{
		for(const auto &x : paths)
		{
			this->paths.emplace_back(x);
		}
	}

	template <class T>
	bool TPool<T>::serialize() noexcept
	{
		if (serializator && components.size() > 0)
		{
			for (const auto &x : components)
			{
				serializator(x);
			}
			return true;
		}
		return false;
	}

	template <class T>
	bool TPool<T>::deserialize() noexcept
	{
		if (deserializator && paths.size() > 0)
		{
			for (const auto path : paths)
			{
				auto comp = deserializator(path);
				components.emplace_back(comp);
			}
			return true;
		}
		return false;
	}

	template <class T>
	void TPool<T>::addSerializator(std::function<void(std::shared_ptr<T>& comp)>& serializator) noexcept
	{
		this->serializator = serializator;
	}

	template <class T>
	void TPool<T>::addDeserializator(std::function<std::shared_ptr<T>(const std::string& path)>& deserializator) noexcept
	{
		this->deserializator = deserializator;
	}

	template <class T>
	void TPool<T>::removeSerializator() noexcept
	{
		serializator = nullptr;
	}

	template <class T>
	void TPool<T>::removeDeserializator() noexcept
	{
		serializator = nullptr;
	}

	template <class T>
	std::function<std::shared_ptr<T>(const std::string& path)> TPool<T>::getDeserializator() noexcept
	{
		return deserializator;
	}

	template <class T>
	std::function<void(std::shared_ptr<T>& comp)> TPool<T>::getSerializator() noexcept
	{
		return serializator;
	}


	template <class T>
	void TPool<T>::add(std::shared_ptr<T>& comp) noexcept
	{
		components.emplace_back(comp);
	}

	template <class T>
	bool TPool<T>::remove(std::shared_ptr<T>& comp) noexcept
	{
		auto find = std::find(begin(components), end(components), comp);
		if (find != end(components))
		{
			std::swap(components.back(), *find);
			components.pop_back();
			return true;
		}
		return false;
	}

	template <class T>
	std::vector<ECS::Entity*> TPool<T>::getEntities() noexcept
	{

		std::vector<ECS::Entity*> entities(components.size());
		for (const auto &comp : components)
		{
			entities.emplace_back(comp->entity);
		}
		return std::move(entities);
	}

	template <class T>
	std::vector<std::shared_ptr<T>>& TPool<T>::getComponents() noexcept
	{
		return components;
	}

}
