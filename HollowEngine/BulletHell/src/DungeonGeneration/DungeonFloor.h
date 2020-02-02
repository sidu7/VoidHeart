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
		int GetRoomCount() const { return mIndexValidRooms.size(); }
		const DungeonRoom& GetRoomFromIndex(int index) const { return mRooms[mIndexValidRooms[index]]; }
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
		std::vector<int> mIndexValidRooms; // stores index to valid rooms 
        int mWidth;
        int mHeight;
        int mFloorNum;
        int mEntranceIndex;
    };
}