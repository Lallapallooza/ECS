#pragma once

namespace ECS
{
	class BaseEntity;

	class BaseComponent
	{
	public:
		BaseEntity *entity;
		virtual ~BaseComponent(){};
		BaseComponent()
		{
			this->entity = nullptr;
		};
	};

}
