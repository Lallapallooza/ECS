#pragma once

namespace ECS
{
	class BaseSystem
	{
	public:
		BaseSystem();
		static BaseSystem& instance();
		virtual void update();
		virtual ~BaseSystem();
	protected:
		virtual void subscribe();
	};

}