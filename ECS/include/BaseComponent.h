#pragma once

namespace ECS
{
	class BaseEntity;

	class BaseComponent
	{
	public:
	    BaseEntity* getEntity() const noexcept
	    {
			return entity;
	    };
		virtual ~BaseComponent();
		BaseComponent();
	protected:
		BaseEntity *entity;
	};

}
