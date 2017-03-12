#include "stdafx.h"
#include "BaseSystem.h"
#include "Tools.h"

ECS::BaseSystem::BaseSystem()
{
}

ECS::BaseSystem& ECS::BaseSystem::instance()
{
	static BaseSystem s;
	return s;
}

void ECS::BaseSystem::update()
{
}


ECS::BaseSystem::~BaseSystem()
{
}

void ECS::BaseSystem::subscribe()
{
}
