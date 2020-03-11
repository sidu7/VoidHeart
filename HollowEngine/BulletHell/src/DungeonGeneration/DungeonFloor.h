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
		int GetFloorNumber() const { return mFloorNum; }
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
        Hollow::GameObject* CreateMinimapElement(const glm::vec2& position, const glm::vec2& scale, float angle, const std::string texturePath, bool isVisible);
        void SetUIRoomTexture(std::string path, int index, bool isVisible); // index assumed to be valid
        void SetUINeighborRoomsTexture(std::string path, int index, bool isVisible); // index assumed to be valid
        void TrySetUIRoomTexture(std::string path, int index, bool isVisible); // index assumed to be valid
        void TrySetUINeighborRoomsTexture(std::string path, int index, bool isVisible); // index assumed to be valid
        void CreateMinimap();
        Hollow::GameObject* CreateMinimapRoomAt(int roomIndex);
        void OnRoomEnter(int newIndex, int oldIndex);
        std::vector<DungeonRoom>& GetRooms() { return mRooms; }
    private:
        std::vector<DungeonRoom> mRooms; // 1D representation of a 2D array
		std::vector<int> mIndexRegularRooms; // stores index to regular rooms (No Entrance/Boss) 
        Hollow::GameObject* mpMinimapPlayer;
        int mWidth;
        int mHeight;
        int mFloorNum;
        int mEntranceIndex;
        int mBossIndex;

        const std::string mMinimapFoggedRoomPath = "Resources/Textures/Minimap/UIMinimapFoggedRoom.png";
        const std::string mMinimapCurrentRoomPath = "Resources/Textures/Minimap/UIMinimapPlayer.png";
    };
}