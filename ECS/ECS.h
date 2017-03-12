#pragma once

#ifdef ECS_EXPORTS
#define ECS_API __declspec(dllexport) 
#else
#define ECS_API __declspec(dllimport) 
#endif

namespace ECS
{
	class BaseComponent;
	class BaseSystem;
	class BaseEntity;
	class Pool;
}