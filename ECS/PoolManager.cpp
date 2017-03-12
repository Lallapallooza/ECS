#include "stdafx.h"
#include "PoolManager.h"


ECS::PoolManager& ECS::PoolManager::instance() noexcept
{
	static PoolManager pool;
	return pool;
}

ECS::PoolManager::~PoolManager()
{
	
}


ECS::PoolManager::PoolManager()
{
}
