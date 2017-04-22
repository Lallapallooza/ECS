#pragma once

namespace ECS
{
	class Entity;

	class BaseComponent
	{
	public:
		Entity *entity;

		virtual ~BaseComponent(){};
		BaseComponent()
		{
			this->entity = nullptr;
		};
	};

}
