#pragma once
#include "GameMetaData/DungeonRoomType.h"
#include <glm/glm.hpp>
#include "Hollow.h"
#include "Hollow/Core/GameObject.h"

namespace BulletHell
{
    class DungeonFloor;
    class DungeonManager;
    class DungeonRoom
    {
        friend DungeonFloor;
    public:
        enum DoorDirrection
        {
            NONE = 0,
            UP = 1,
            RIGHT = 2,
            DOWN = 4,
            LEFT = 8
        };

        DungeonRoom();
        DungeonRoom(DungeonRoomType roomType, int roomID, int doors
            , int floorNum, int xCoord, int yCoord);

        void Set(DungeonRoomType roomType, int roomID, int doors
            , int floorNum, int xCoord, int yCoord);
        DungeonRoom& operator=(const DungeonRoom& room);
        glm::ivec2 GetCoords() const;
		int GetFloorNum() const { return mFloorNum; }
    private:
        int TotalDoors() const;
    public:
        void ConstructRoom();

        static float mWallLength;
        static float mWallHeight;
        static float mRoomSize;
        static float mWallThickness;
        static float mDoorWidth;
        static float mDoorHeight;
        static float mDoorThickness;
        int mEnemyCount;
        std::vector<Hollow::GameObject*> mDoorGOs;
    private:    	
        DungeonRoomType mRoomType;
        int mRoomID;   // unique id of the room
        int mDoors;    // flag field for 4 doors
        int mFloorNum; // floor number of the room (owner id)
        int mColumn;   // position in the grid of the floor
        int mRow;      // position in the grid of the floor
        int mDistFromEntrance;
    };
}
