#include <Hollow.h>
#include "DungeonRoom.h"

#include "Hollow/Managers/ResourceManager.h"

#include "Hollow/Components/Body.h"

namespace BulletHell
{
    DungeonRoom::DungeonRoom()
        : mRoomType(DungeonRoomType::EMPTY)
        //, mRoomShape(DungeonRoomShape::ONE_ONE)
        , mRoomID(0)
        , mDoors(DoorDirrection::NONE)
        , mFloorNum(0)
        , mXcoord(0)
        , mYcoord(0)
        , mDistFromEntrance(0)
    {
    }

    DungeonRoom::DungeonRoom(DungeonRoomType roomType, int roomID, int doors
        , int floorNum, int xCoord, int yCoord)
        : mRoomType(roomType)
        , mRoomID(roomID)
        , mDoors(doors)
        , mFloorNum(floorNum)
        , mXcoord(xCoord)
        , mYcoord(yCoord)
        , mDistFromEntrance(0)
    {
    }

    void DungeonRoom::Set(DungeonRoomType roomType, int roomID, int doors
        , int floorNum, int xCoord, int yCoord)
    {
        mRoomType = roomType;
        mRoomID = roomID;
        mDoors = doors;
        mFloorNum = floorNum;
        mXcoord = xCoord;
        mYcoord = yCoord;
    }

    DungeonRoom& DungeonRoom::operator=(const DungeonRoom& room)
    {
        mRoomType = room.mRoomType;
        mRoomID = room.mRoomID;
        mDoors = room.mDoors;
        mFloorNum = room.mFloorNum;
        mXcoord = room.mXcoord;
        mYcoord = room.mYcoord;
        mDistFromEntrance = room.mDistFromEntrance;
        return *this;
    }

    int DungeonRoom::TotalDoors() const
    {
        // have to cast to boolean because DOWN==2 and LEFT==4
        // so when I add the results of & operator it might be more then 4 doors
        bool a = mDoors & DoorDirrection::UP;
        bool b = mDoors & DoorDirrection::RIGHT;
        bool c = mDoors & DoorDirrection::DOWN;
        bool d = mDoors & DoorDirrection::LEFT;

        return a + b + c + d; // adding booleans and casting to int implicitly
    }

    void DungeonRoom::ConstructRoom()
    {
        // Programatically create room walls and doors
        // Do this for all rooms, or when desired for a specific room
        // {
        /*	NONE = 0,
                UP = 1,
                RIGHT = 2,
                DOWN = 4,
                LEFT = 8
        */
      
    	
        //E.x. room at 3,2 in floor grid with doors at left and top
            int mRoomX = mXcoord;
            int mRoomY = mYcoord;
            int mDoorBits = mDoors;

        Hollow::GameObject* pRoomFloor = 
            Hollow::ResourceManager::Instance().LoadPrefabAtPosition("Floor", glm::vec3(mRoomY*15 + 7.5, 0.0f, mRoomX * 15 + 7.5));
       
    	// Origin of room is in top left corner
            const int numRows = 15;
            const int numCols = 15;
            const int halfRow = 7; // floor(numRows/2)
            const int halfCol = 7; // floor(numCols/2)
            for (int rowIndex = 0; rowIndex < numRows; ++rowIndex)
            {
                for (int colIndex = 0; colIndex < numCols; ++colIndex)
                {
                    if (colIndex % (numCols - 1) == 0 || rowIndex % (numRows - 1) == 0)
                    {
                        // Check door positions
                        // UP
                        if ((colIndex == halfCol) && (rowIndex == 0) && (mDoorBits & 1))
                        {
                            // Create up door
                        }
                        else if ((colIndex == 0) && (rowIndex == halfRow) && (mDoorBits & 8))
                        {
                            // Create left door
                        }
                        else if ((rowIndex == halfRow) && (colIndex == numCols - 1) && (mDoorBits & 2))
                        {
                            // Create right door
                        }
                        else if ((rowIndex == numRows - 1) && (colIndex == halfRow) && (mDoorBits & 4))
                        {
                            // Create down door
                            int dummy = rowIndex + colIndex;
                        }
                        else
                        {
                            Hollow::ResourceManager::Instance().LoadPrefabAtPosition("Wall", glm::vec3(colIndex + mRoomY * 15, 0.0f, rowIndex + mRoomX * 15));
                        }
                    }
                }
            }
    }
}
