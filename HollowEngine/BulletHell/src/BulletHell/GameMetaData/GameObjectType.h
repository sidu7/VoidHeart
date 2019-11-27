#pragma once

namespace BulletHell
{
	enum class GameObjectType
	{
#define GAMEOBJECT_TYPE(name) name,
#include "GameObjectType.enum"
#undef GAMEOBJECT_TYPE
		NUM
	};
}