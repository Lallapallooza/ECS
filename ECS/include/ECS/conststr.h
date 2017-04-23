#pragma once
#include <stdexcept>

class conststr
{
private:
	const char* p;
	const int size_;
public:
	template<std::size_t N>
	constexpr conststr(const char(&a)[N]) : p(a), size_(N - 1) {}
	constexpr char operator[](std::size_t n) const
	{
		return n < size_ ? p[n] : throw std::out_of_range("");
	}
	constexpr int size() const { return size_; }
};

constexpr int g_(conststr s, int i = 0, unsigned int u_id = 1)
{
	return s.size() == i ? u_id :
		s[i] >= 10 && s[i] <= 99 ? g_(s, i + 1, u_id = u_id * 10 + s[i] / 100) :
		s[i] > 100 ? g_(s, i + 1, u_id = u_id * 100 + s[i] / 100) :
		g_(s, i + 1, u_id += s[i] / 10);
}
