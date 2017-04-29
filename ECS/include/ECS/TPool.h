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
		
		static void addSerializator(std::function<void(std::shared_ptr<T> &comp)> serializator) noexcept;
		static void addDeserializator(std::function<std::shared_ptr<T>(const std::string &path)> deserializator) noexcept;

		static void removeSerializator() noexcept;
		static void removeDeserializator() noexcept;

		static std::function<std::shared_ptr<T>(const std::string &path)> getDeserializator() noexcept;
		static std::function<void(std::shared_ptr<T> &comp)> getSerializator() noexcept;
	private:
		
		static std::vector<std::string> paths;
		static std::function<void(std::shared_ptr<T> &comp)> _serializator;
		static std::function<std::shared_ptr<T>(const std::string &path)> _deserializator;
		static std::vector<std::shared_ptr<T>> components;
	};

	template<class T>
	std::vector<std::shared_ptr<T>> TPool<T>::components = {};

	template<class T>
	std::function<std::shared_ptr<T>(const std::string &path)> TPool<T>::_deserializator = nullptr;


	template<class T>
	std::function<void(std::shared_ptr<T> &comp)> TPool<T>::_serializator = nullptr;

	template<class T>
	std::vector<std::string> TPool<T>::paths = {};

	template<class T>
	unsigned int TPool<T>::id = -1;

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
		if (_serializator && components.size() > 0)
		{
			for (auto &x : components)
			{
				_serializator(x);
			}
			return true;
		}
		return false;
	}

	template <class T>
	bool TPool<T>::deserialize() noexcept
	{
		if (_deserializator && paths.size() > 0)
		{
			for (const auto path : paths)
			{
				auto comp = _deserializator(path);
				components.emplace_back(comp);
			}
			return true;
		}
		return false;
	}

	template <class T>
	void TPool<T>::addSerializator(std::function<void(std::shared_ptr<T>& comp)> serializator) noexcept
	{
		_serializator = serializator;
	}

	template <class T>
	void TPool<T>::addDeserializator(std::function<std::shared_ptr<T>(const std::string& path)> deserializator) noexcept
	{
		_deserializator = deserializator;
	}

	template <class T>
	void TPool<T>::removeSerializator() noexcept
	{
		_serializator = nullptr;
	}

	template <class T>
	void TPool<T>::removeDeserializator() noexcept
	{
		_serializator = nullptr;
	}

	template <class T>
	std::function<std::shared_ptr<T>(const std::string& path)> TPool<T>::getDeserializator() noexcept
	{
		return _deserializator;
	}

	template <class T>
	std::function<void(std::shared_ptr<T>& comp)> TPool<T>::getSerializator() noexcept
	{
		return _serializator;
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
