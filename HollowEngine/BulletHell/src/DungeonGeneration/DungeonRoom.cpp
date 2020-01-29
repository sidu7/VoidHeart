#include "DungeonRoom.h"

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
}