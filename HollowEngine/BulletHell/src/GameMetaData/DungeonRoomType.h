#pragma once

namespace BulletHell
{
	enum class DungeonRoomType
	{
#define DUNGEON_ROOM_TYPE(name) name,
#include "DungeonRoomType.enum"
#undef DUNGEON_ROOM_TYPE
		NUM
	};
}