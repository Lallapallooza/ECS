#pragma once

#include "BaseEntity.h"
#include <memory>
#include <vector>


class conststr
{
private:
	const char* p;
	const int size_;
public:
	template<std::size_t N>
	constexpr conststr(const char(&a)[N]) : p(a), size_(N - 1){}
	constexpr char operator[](std::size_t n) const
	{
		return n < size_ ? p[n] : throw std::out_of_range("");
	}
	constexpr int size() const { return size_; }
};

constexpr int g_(conststr s, int i = 0, int u_id = 1)
{
	return s.size() == i ? u_id :
						 s[i] >= 10 & s[i] <= 99 ? g_(s, i + 1, u_id *= 100 + s[i]) :
												   s[i] > 100 ? g_(s, i + 1, u_id *= 1000 + s[i]) :
																g_(s, i + 1, u_id *= 10 + s[i]);
}

namespace ECS
{

	template<int N>
	class Group
	{
	public:
		template<class Ty>
		static void add(std::shared_ptr<Ty> &entity);
		template<class Ty>
		static void remove(std::shared_ptr<Ty> &entity);

		static std::vector<std::shared_ptr<ECS::BaseEntity>>& getEntities();

	private:
		static std::vector<std::shared_ptr<ECS::BaseEntity>> entities;
	};

	template<int N>
	std::vector<std::shared_ptr<ECS::BaseEntity>> Group<N>::entities = {};

	template<int N>
	template<class Ty>
	void Group<N>::add(std::shared_ptr<Ty>& entity)
	{
		entities.push_back(std::static_pointer_cast<ECS::BaseEntity>(entity));
	}

	template <int N>
	template<class Ty>
	void Group<N>::remove(std::shared_ptr<Ty>& entity)
	{
		auto find = std::find(begin(entities), end(entities), std::static_pointer_cast<ECS::BaseEntity>(entity));
		if (find != end(entities))
		{
			std::swap(entities.back(), *find);
			entities.pop_back();
		}
	}

	template <int N>
	std::vector<std::shared_ptr<ECS::BaseEntity>>& Group<N>::getEntities()
	{
		return entities;
	}
}