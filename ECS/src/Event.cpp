#include "Event.h"

ECS::tools::Event<void> ECS::preUpdate = {};
ECS::tools::Event<void> ECS::Update = {};
ECS::tools::Event<void> ECS::lateUpdate = {};