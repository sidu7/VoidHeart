#pragma once

	enum GameObjectType
	{
#define GAMEOBJECT_TYPE(name) name,
#include "GameObjectType.enum"
#undef GAMEOBJECT_TYPE
		NUM
	};

