#include "Tools.h"

std::vector<int> ECS::tools::Collector::ids = {};

ECS::tools::Event ECS::preUpdate = {};
ECS::tools::Event ECS::Update = {};
ECS::tools::Event ECS::lateUpdate = {};