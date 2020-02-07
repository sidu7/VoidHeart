#pragma once
#include "GameMetaData/DungeonRoomType.h"
#include <glm/glm.hpp>

namespace BulletHell
{
    class DungeonFloor;
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
    private:    	
        DungeonRoomType mRoomType;
        //DungeonRoomShape mRoomShape; // ALEKSEY: delete this if we never use this
        int mRoomID;	 // unique id of the room
        int mDoors;    // flag field for 4 doors
        int mFloorNum; // floor number of the room (owner id)
        int mXcoord;   // position in the grid of the floor
        int mYcoord;   // position in the grid of the floor
        int mDistFromEntrance;
    };
}
