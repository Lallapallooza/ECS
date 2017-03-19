#pragma once

namespace ECS
{
	class BaseSystem
	{
	public:
		BaseSystem(){};
		static BaseSystem& instance();
		virtual void update();
		virtual ~BaseSystem(){};
		virtual void subscribe();
	};

	inline BaseSystem& BaseSystem::instance()
	{
		static BaseSystem s;
		return s;
	}
}
