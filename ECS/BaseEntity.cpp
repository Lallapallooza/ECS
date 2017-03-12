#include "stdafx.h"
#include "BaseEntity.h"


ECS::BaseEntity::BaseEntity()
{
}

std::list<std::shared_ptr<ECS::BaseComponent>>& ECS::BaseEntity::getComponents()
{
	return components;
}


ECS::BaseEntity::~BaseEntity()
{
}
