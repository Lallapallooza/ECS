#include "stdafx.h"
#include "BaseComponent.h"
#include "BaseEntity.h"
#include "TPool.h"
#include "PoolManager.h"


ECS::BaseComponent::~BaseComponent()
{
}

ECS::BaseComponent::BaseComponent()
{
	this->entity = nullptr;
}

