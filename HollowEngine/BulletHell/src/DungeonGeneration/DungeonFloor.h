#pragma once
#include "DungeonRoom.h"
#include "hollowpch.h"

namespace BulletHell
{
    class DungeonFloor
    {
    public:
        DungeonFloor();
        DungeonFloor(int width, int height, int floorNum);
        void Generate(int numRooms);
        DungeonRoom& GetRoom(int x, int y);
        void ResetFloor();

        void PrintFloor(int printMode = 1) const;
    private:
        int Index(int row, int col) const; // helper function to convert 2D index to 1D
        void Index2D(int index1D, int& row, int& col);
        int SumDistance(std::vector<int>); // all the distances from the entrance combine, used to calculate weight percentage to pick a room
        void UpdateDoors(int roomIndex);

    public:

    private:
        std::vector<DungeonRoom> mRooms; // 1D representation of a 2D array
        int mWidth;
        int mHeight;
        int mFloorNum;

    };
}