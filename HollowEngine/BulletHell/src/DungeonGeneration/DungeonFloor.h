#pragma once
#include <vector>
#include <iostream>
#include "DungeonRoom.h"

namespace BulletHell
{
    class DungeonFloor
    {
    public:
        DungeonFloor();
        DungeonFloor(int width, int height, int floorNum);
        void Generate(int numRooms, unsigned seed);
        DungeonRoom& GetRoom(int row, int col);
        void ResetFloor();
        const DungeonRoom& GetEntrance() const;
        int GetEntranceIndex();
        int GetBossIndex() { return mBossIndex; }
		int GetRoomCount() const { return mIndexRegularRooms.size() + 2; }
		DungeonRoom& GetRoomFromIndex(int index);
		const DungeonRoom& GetRegularRoom(int index) const;
        void PrintFloor(int printMode = 1) const;
    private:
        int Index(int row, int col) const; // helper function to convert 2D index to 1D
        void Index2D(int index1D, int& row, int& col);
        int SumDistance(std::vector<int>* roomIndices); // all the distances from the entrance combine, used to calculate weight percentage to pick a room
        void UpdateDoors(int roomIndex);
        void UpdateDoor(int parentRoomIndex, int neighborRoomIndex, DungeonRoom::DoorDirrection parentDir, DungeonRoom::DoorDirrection neighborDir);

        void UpdatePossibleRooms(std::vector<int>* possibleRooms, int row, int col);
        void CheckAndAddNeighborOf(std::vector<int>* possibleRooms, int neighborIndex);
    
    public:
        void ConstructFloor();

    private:
        std::vector<DungeonRoom> mRooms; // 1D representation of a 2D array
		std::vector<int> mIndexRegularRooms; // stores index to regular rooms (No Entrance/Boss) 
        int mWidth;
        int mHeight;
        int mFloorNum;
        int mEntranceIndex;
        int mBossIndex;
    };
}