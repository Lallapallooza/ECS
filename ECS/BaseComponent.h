#pragma once

namespace ECS
{
	class BaseEntity;

	class BaseComponent
	{
	public:
		virtual BaseEntity& getEntity() = 0;
		virtual ~BaseComponent();
	protected:
		BaseEntity *entity;
	private:
		BaseComponent() = delete;
	};

}
