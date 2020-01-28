#include "DungeonFloor.h"
#include "Hollow/Utils/Random.h"
#include "Hollow/Utils/UniqueID.h"

namespace BulletHell
{
    DungeonFloor::DungeonFloor()
        : mWidth(0)
        , mHeight(0)
        , mFloorNum(0)
    {
    }

    DungeonFloor::DungeonFloor(int width, int height, int floorNum)
        : mWidth(width)
        , mHeight(height)
        , mFloorNum(floorNum)
    {
        int size = width * height;
        mRooms.reserve(size);
        for (int i = 0; i < size; i++)
        {
            mRooms.emplace_back();
        }
    }

    void DungeonFloor::Generate(int numRooms)
    {
        if (numRooms < 2)
        {
            std::cout << "Error! Dungeon Floor Generator: Less then 2 rooms" << std::endl;
            return;
        }
        if (numRooms > mWidth * mHeight)
        {
            std::cout << "Error! Dungeon Floor Generator: To many rooms, to little dungeon floor size" << std::endl;
            return;
        }

        ////// Room generation

        //// entrance room
        unsigned seed = 521288629;
        Random::SetSeed(seed);
        auto entranceRow = Random::RangeSeeded(0, mHeight - 1);
        auto entranceCol = Random::RangeSeeded(0, mWidth - 1);
        const int entranceIndex = Index(entranceRow, entranceCol);
        mRooms[entranceIndex].Set(DungeonRoomType::ENTRANCE, Hollow::GenerateUniqueID<DungeonRoom>(), 0, mFloorNum, entranceRow, entranceCol);

        // precompute distance of other rooms from entrance
        for (int col = 0; col < mWidth; col++)
        {
            for (int row = 0; row < mHeight; row++)
            {
                mRooms[Index(row, col)].mDistFromEntrance = std::abs(row - entranceRow) + std::abs(col - entranceCol);
            }
        }

        // generate a pool of rooms to pick from next
        std::vector<int> possibleRoomLocation; // pool of rooms that could be added to the dungeon(indexis to 1D array of rooms)        
        if (entranceCol > 0)
        {
            int roomCandidateIndex = Index(entranceRow, entranceCol - 1); // index of a room that might be added to the pool of room to pick from
            possibleRoomLocation.push_back(roomCandidateIndex); // left
            mRooms[roomCandidateIndex].mRoomType = DungeonRoomType::PENDING_GENERATION;
        }
        if (entranceRow < mHeight - 1)
        {
            int roomCandidateIndex = Index(entranceRow + 1, entranceCol); // index of a room that might be added to the pool of room to pick from
            possibleRoomLocation.push_back(roomCandidateIndex); // down
            mRooms[roomCandidateIndex].mRoomType = DungeonRoomType::PENDING_GENERATION;
        }
        if (entranceCol < mWidth - 1)
        {
            int roomCandidateIndex = Index(entranceRow, entranceCol + 1); // index of a room that might be added to the pool of room to pick from
            possibleRoomLocation.push_back(roomCandidateIndex); // right
            mRooms[roomCandidateIndex].mRoomType = DungeonRoomType::PENDING_GENERATION;
        }
        if (entranceRow > 0)
        {
            int roomCandidateIndex = Index(entranceRow - 1, entranceCol); // index of a room that might be added to the pool of room to pick from
            possibleRoomLocation.push_back(roomCandidateIndex); // up
            mRooms[roomCandidateIndex].mRoomType = DungeonRoomType::PENDING_GENERATION;
        }
        //// rest of the rooms
        for (int i = 1; i < numRooms; i++)
        {
            // pick a room location from pool of possible places

            int row, col;
            int currentRoomIndex;
            int possibleRoomIndex;
            while (true)
            {
                possibleRoomIndex = Random::RangeSeeded(0, possibleRoomLocation.size() - 1);
                currentRoomIndex = possibleRoomLocation[possibleRoomIndex];
                Index2D(currentRoomIndex, row, col);

                // check neighbors as well to make sure to cause them to overflow
                int upIndex = (row > 0) ? Index(row - 1, col) : -1;
                int rightIndex = (col < mWidth - 1) ? Index(row, col + 1) : -1;
                int downIndex = (row < mHeight - 1) ? Index(row + 1, col) : -1;
                int leftIndex = (col > 0) ? Index(row, col - 1) : -1;

                // doors allowed per room
                const int maxDoors = 3;
                if ((upIndex != -1 && mRooms[upIndex].TotalDoors() == maxDoors)
                    || (rightIndex != -1 && mRooms[rightIndex].TotalDoors() == maxDoors)
                    || (downIndex != -1 && mRooms[downIndex].TotalDoors() == maxDoors)
                    || (leftIndex != -1 && mRooms[leftIndex].TotalDoors() == maxDoors))
                {
                    mRooms[currentRoomIndex].mRoomType = DungeonRoomType::EMPTY_PROCESSED;
                    continue;
                }
                break;
            }
            mRooms[currentRoomIndex].Set(DungeonRoomType::REGULAR, Hollow::GenerateUniqueID<DungeonRoom>(), 0, mFloorNum, row, col);
            UpdateDoors(currentRoomIndex);

            // update the pool of possible places for the rooms 
            possibleRoomLocation.erase(possibleRoomLocation.begin() + possibleRoomIndex); // we just got a room at this pos

            if (row > 0 && mRooms[Index(row - 1, col)].mRoomType == DungeonRoomType::EMPTY)
            {
                int roomCandidateIndex = Index(row - 1, col);
                possibleRoomLocation.push_back(Index(row - 1, col)); // up
                mRooms[roomCandidateIndex].mRoomType = DungeonRoomType::PENDING_GENERATION;
            }
            if (col < mWidth - 1 && mRooms[Index(row, col + 1)].mRoomType == DungeonRoomType::EMPTY)
            {
                int roomCandidateIndex = Index(row, col + 1);
                possibleRoomLocation.push_back(Index(row, col + 1)); // right
                mRooms[roomCandidateIndex].mRoomType = DungeonRoomType::PENDING_GENERATION;
            }
            if (row < mHeight - 1 && mRooms[Index(row + 1, col)].mRoomType == DungeonRoomType::EMPTY)
            {
                int roomCandidateIndex = Index(row + 1, col);
                possibleRoomLocation.push_back(Index(row + 1, col)); // down
                mRooms[roomCandidateIndex].mRoomType = DungeonRoomType::PENDING_GENERATION;
            }
            if (col > 0 && mRooms[Index(row, col - 1)].mRoomType == DungeonRoomType::EMPTY)
            {
                int roomCandidateIndex = Index(row, col - 1);
                possibleRoomLocation.push_back(roomCandidateIndex); // left
                mRooms[roomCandidateIndex].mRoomType = DungeonRoomType::PENDING_GENERATION;
            }
        }

        // reset pending rooms
        for (int roomIndex : possibleRoomLocation)
        {
            mRooms[roomIndex].mRoomType = DungeonRoomType::EMPTY;
        }

        //// boss room
        // find furthest away room from the entrance, assign it to boss room 
        // find furthest away dead-end for the boss room
        int biggestDist = 0;
        int furthestRoom = entranceIndex;
        for (unsigned i = 0; i < mRooms.size(); i++)
        {
            const DungeonRoom& room = mRooms[i];
            const int numDoors = room.TotalDoors();
            if (numDoors == 1)
            {
                if (room.mDistFromEntrance > biggestDist)
                {
                    biggestDist = room.mDistFromEntrance;
                    furthestRoom = i;
                }
            }
        }

        mRooms[furthestRoom].mRoomType = DungeonRoomType::BOSS;
    }

    DungeonRoom& DungeonFloor::GetRoom(int x, int y)
    {
        return mRooms.at(x * mWidth + y);
    }

    void DungeonFloor::ResetFloor()
    {
        for (DungeonRoom& room : mRooms)
        {
            room.mRoomType = DungeonRoomType::EMPTY;
            room.mDoors = 0;
        }
    }

    void DungeonFloor::PrintFloor(int printMode) const
    {
        if (printMode & 1)
        {
            for (int i = 0; i < mHeight; i++)
            {
                for (int j = 0; j < mWidth; j++)
                {
                    const DungeonRoom& room = mRooms[Index(i, j)];
                    DungeonRoomType roomType = room.mRoomType;
                    if (roomType == DungeonRoomType::REGULAR)
                        std::cout << "* ";
                    else if (roomType == DungeonRoomType::ENTRANCE)
                        std::cout << "& ";
                    else if (roomType == DungeonRoomType::BOSS)
                        std::cout << "! ";
                    else if (roomType == DungeonRoomType::EMPTY)
                        std::cout << "  ";
                    else if (roomType == DungeonRoomType::EMPTY_PROCESSED)
                        std::cout << ". ";
                    else if (roomType == DungeonRoomType::PENDING_GENERATION)
                        std::cout << "+ ";
                }
                std::cout << std::endl;
            }
            //std::cout << std::endl;
            std::cout << "* - regular room" << std::endl;
            std::cout << "& - entrance room" << std::endl;
            std::cout << "! - boss room" << std::endl;
            std::cout << std::endl;
        }
        if (printMode & 2)
        {
            for (int i = 0; i < mHeight; i++)
            {
                for (int j = 0; j < mWidth; j++)
                {
                    const DungeonRoom& room = mRooms[Index(i, j)];
                    DungeonRoomType roomType = room.mRoomType;
                    std::cout << room.mRoomID << " ";

                }
                std::cout << std::endl;
            }
        }
        if (printMode & 4)
        {
            for (int i = 0; i < mHeight; i++)
            {
                for (int j = 0; j < mWidth; j++)
                {
                    const DungeonRoom& room = mRooms[Index(i, j)];
                    int dist = room.mDistFromEntrance;
                    std::cout << dist << " ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
        if (printMode & 8)
        {
            for (int i = 0; i < mHeight; i++)
            {
                for (int j = 0; j < mWidth; j++)
                {
                    const DungeonRoom& room = mRooms[Index(i, j)];
                    int totalDoors = room.TotalDoors();
                    std::cout << totalDoors << " ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
        if (printMode & 16) // dead-ends
        {
            for (int i = 0; i < mHeight; i++)
            {
                for (int j = 0; j < mWidth; j++)
                {
                    const DungeonRoom& room = mRooms[Index(i, j)];
                    if (room.mRoomType == DungeonRoomType::ENTRANCE)
                        std::cout << "& ";
                    else if (room.TotalDoors() == 1)
                        std::cout << "e ";
                    else
                        std::cout << "  ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
    }

    int DungeonFloor::Index(int row, int col) const
    {
        return row * mWidth + col;
    }

    void DungeonFloor::Index2D(int index1D, int& row, int& col)
    {
        row = index1D / mWidth;
        col = index1D % mWidth;
    }

    int DungeonFloor::SumDistance(std::vector<int> roomIndices)
    {
        int sum = 0;
        for (int i : roomIndices)
        {
            sum += mRooms[i].mDistFromEntrance;
        }
        return sum;
    }

    void DungeonFloor::UpdateDoors(int roomIndex)
    {
        int row, col;
        Index2D(roomIndex, row, col);

        int upIndex = (row > 0) ? Index(row - 1, col) : -1;
        int rightIndex = (col < mWidth - 1) ? Index(row, col + 1) : -1;
        int downIndex = (row < mHeight - 1) ? Index(row + 1, col) : -1;
        int leftIndex = (col > 0) ? Index(row, col - 1) : -1;

        //up
        if (upIndex >= 0
            && (mRooms[upIndex].mRoomType == DungeonRoomType::ENTRANCE || mRooms[upIndex].mRoomType == DungeonRoomType::REGULAR))
        {
            mRooms[roomIndex].mDoors |= DungeonRoom::DoorDirrection::UP;
            mRooms[upIndex].mDoors |= DungeonRoom::DoorDirrection::DOWN;
        }
        //right
        if (rightIndex >= 0
            && (mRooms[rightIndex].mRoomType == DungeonRoomType::ENTRANCE || mRooms[rightIndex].mRoomType == DungeonRoomType::REGULAR))
        {
            mRooms[roomIndex].mDoors |= DungeonRoom::DoorDirrection::RIGHT;
            mRooms[rightIndex].mDoors |= DungeonRoom::DoorDirrection::LEFT;
        }
        //down
        if (downIndex >= 0
            && (mRooms[downIndex].mRoomType == DungeonRoomType::ENTRANCE || mRooms[downIndex].mRoomType == DungeonRoomType::REGULAR))
        {
            mRooms[roomIndex].mDoors |= DungeonRoom::DoorDirrection::DOWN;
            mRooms[downIndex].mDoors |= DungeonRoom::DoorDirrection::UP;
        }
        //left
        if (leftIndex >= 0
            && (mRooms[leftIndex].mRoomType == DungeonRoomType::ENTRANCE || mRooms[leftIndex].mRoomType == DungeonRoomType::REGULAR))
        {
            mRooms[roomIndex].mDoors |= DungeonRoom::DoorDirrection::LEFT;
            mRooms[leftIndex].mDoors |= DungeonRoom::DoorDirrection::RIGHT;
        }
    }
}