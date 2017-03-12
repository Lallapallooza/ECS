#pragma once

namespace ECS
{
	class BaseEntity;

	class BaseComponent
	{
	public:
		virtual BaseEntity& getEntity() = 0;
		virtual ~BaseComponent();
		BaseComponent();
	protected:
		BaseEntity *entity;
	};

}
