#include "DungeonFloor.h"
#include "Hollow/Utils/Random.h"
#include "Hollow/Utils/UniqueID.h"

namespace BulletHell
{
	DungeonFloor::DungeonFloor()
		: mWidth(0)
		, mHeight(0)
		, mFloorNum(0)
		, mEntranceIndex(-1)
	{
	}

	DungeonFloor::DungeonFloor(int width, int height, int floorNum)
		: mWidth(width)
		, mHeight(height)
		, mFloorNum(floorNum)
	{
		const int size = width * height;
		mRooms.reserve(size);
		for (int i = 0; i < size; i++)
		{
			mRooms.emplace_back();
		}
	}

	void DungeonFloor::Generate(int numRooms, unsigned seed)
	{
		if (numRooms < 2)
		{
			std::cout << "Error! Dungeon Floor Generator: Less then 2 rooms" << std::endl;
			return;
		}
		if (numRooms > mWidth* mHeight)
		{
			std::cout << "Error! Dungeon Floor Generator: To many rooms, to little dungeon floor size" << std::endl;
			return;
		}

		Random::SetSeed(seed);

		////// Room generation
		//// entrance room
		auto entranceRow = Random::RangeSeeded(0, mHeight - 1);
		auto entranceCol = Random::RangeSeeded(0, mWidth - 1);
		mEntranceIndex = Index(entranceRow, entranceCol);
		mRooms[mEntranceIndex].Set(DungeonRoomType::ENTRANCE, Hollow::GenerateUniqueID<DungeonRoom>(), 0, mFloorNum, entranceRow, entranceCol);

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
		UpdatePossibleRooms(&possibleRoomLocation, entranceRow, entranceCol);

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
			mIndexRegularRooms.push_back(currentRoomIndex);
			UpdateDoors(currentRoomIndex);

			// update the pool of possible places for the rooms 
			possibleRoomLocation.erase(possibleRoomLocation.begin() + possibleRoomIndex); // we just got a room at this pos

			UpdatePossibleRooms(&possibleRoomLocation, row, col);
		}

		// reset pending rooms
		for (int roomIndex : possibleRoomLocation)
		{
			mRooms[roomIndex].mRoomType = DungeonRoomType::EMPTY;
		}

        //// boss room
        // find furthest away room from the entrance, assign it to boss room 
        // find furthest away dead-end for the boss room
        int biggestDistDeadEnd = 0;
        int furthestDeadEndRoom = mEntranceIndex;
        int biggestDist = 0;
        int furthestRoom = mEntranceIndex;
        for (unsigned i = 0; i < mRooms.size(); i++)
        {
            const DungeonRoom& room = mRooms[i];
            const int numDoors = room.TotalDoors();
            if (numDoors == 1)
            {
                if (room.mDistFromEntrance > biggestDistDeadEnd)
                {
                    biggestDistDeadEnd = room.mDistFromEntrance;
                    furthestDeadEndRoom = i;
                }
            }
            else 
            {
                if (room.mRoomType == DungeonRoomType::REGULAR && room.mDistFromEntrance > biggestDist)
                {
                    biggestDist = room.mDistFromEntrance;
                    furthestRoom = i;
                }
            }
        }
        if (furthestDeadEndRoom == mEntranceIndex)
            furthestDeadEndRoom = furthestRoom;

        mRooms[furthestDeadEndRoom].mRoomType = DungeonRoomType::BOSS;
    	mBossIndex = furthestDeadEndRoom;

		// Remove boss room from fillable rooms as that will be handled separately
		mIndexRegularRooms.erase(std::find(mIndexRegularRooms.begin(), mIndexRegularRooms.end(), mBossIndex));

		// shuffle the rooms -- possibly don't need this at all
		//std::random_device rd;
		//std::mt19937 g(rd());
		//std::shuffle(mIndexRegularRooms.begin(), mIndexRegularRooms.end(), g);
	}

	DungeonRoom& DungeonFloor::GetRoom(int row, int col)
	{
		return mRooms.at(row * mWidth + col);
	}

	void DungeonFloor::ResetFloor()
	{
		for (DungeonRoom& room : mRooms)
		{
			room.mRoomType = DungeonRoomType::EMPTY;
			room.mDoors = 0;
		}
		mEntranceIndex = -1;
	}

    const DungeonRoom& DungeonFloor::GetEntrance() const
    {
        if (mEntranceIndex >= 0 && !mRooms.empty())
            return mRooms[mEntranceIndex];
        else
            return DungeonRoom();
    }

	int DungeonFloor::GetEntranceIndex()
	{
		return mEntranceIndex;
	}

	DungeonRoom& DungeonFloor::GetRoomFromIndex(int index)
	{
		return mRooms[index];
	}

	const DungeonRoom& DungeonFloor::GetRegularRoom(int index) const
	{
		return mRooms[mIndexRegularRooms[index]];
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
		if (printMode & 2) // room type
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
		if (printMode & 4) // distance from entrance
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
		if (printMode & 8) // total doors
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

	int DungeonFloor::SumDistance(std::vector<int>* roomIndices)
	{
		int sum = 0;
		for (int i : (*roomIndices))
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

		UpdateDoor(roomIndex, upIndex, DungeonRoom::DoorDirrection::UP, DungeonRoom::DoorDirrection::DOWN);
		UpdateDoor(roomIndex, rightIndex, DungeonRoom::DoorDirrection::RIGHT, DungeonRoom::DoorDirrection::LEFT);
		UpdateDoor(roomIndex, downIndex, DungeonRoom::DoorDirrection::DOWN, DungeonRoom::DoorDirrection::UP);
		UpdateDoor(roomIndex, leftIndex, DungeonRoom::DoorDirrection::LEFT, DungeonRoom::DoorDirrection::RIGHT);
	}

	void DungeonFloor::UpdateDoor(int parentRoomIndex, int neighborRoomIndex, DungeonRoom::DoorDirrection parentDir, DungeonRoom::DoorDirrection neighborDir)
	{
		if (neighborRoomIndex >= 0 &&
			(mRooms[neighborRoomIndex].mRoomType == DungeonRoomType::ENTRANCE || mRooms[neighborRoomIndex].mRoomType == DungeonRoomType::REGULAR))
		{
			mRooms[parentRoomIndex].mDoors |= parentDir;
			mRooms[neighborRoomIndex].mDoors |= neighborDir;
		}
	}

	void DungeonFloor::UpdatePossibleRooms(std::vector<int>* possibleRooms, int row, int col)
	{

		if (row > 0)
		{
			CheckAndAddNeighborOf(possibleRooms, Index(row - 1, col)); // up
		}
		if (col < mWidth - 1)
		{
			CheckAndAddNeighborOf(possibleRooms, Index(row, col + 1)); // right
		}
		if (row < mHeight - 1)
		{
			CheckAndAddNeighborOf(possibleRooms, Index(row + 1, col)); // down
		}
		if (col > 0)
		{
			CheckAndAddNeighborOf(possibleRooms, Index(row, col - 1)); // left
		}
	}

	void DungeonFloor::CheckAndAddNeighborOf(std::vector<int>* possibleRooms, int neighborIndex)
	{
		if (mRooms[neighborIndex].mRoomType == DungeonRoomType::EMPTY)
		{
			(*possibleRooms).push_back(neighborIndex);
			mRooms[neighborIndex].mRoomType = DungeonRoomType::PENDING_GENERATION;
		}
	}

	void DungeonFloor::ConstructFloor()
	{
		for (int i = 0; i < mRooms.size(); i++)
		{
			DungeonRoom& room = mRooms[i];
			if (room.mRoomType != DungeonRoomType::EMPTY)
			{
				room.ConstructRoom();

				// copy already constructed doors to adjacent rooms
				if (room.mDoors & DungeonRoom::DoorDirrection::RIGHT)
				{
					mRooms[i + 1].mDoorGOs[DungeonRoom::DoorDirrection::LEFT]
						= room.mDoorGOs[DungeonRoom::DoorDirrection::RIGHT];
				}
				if (room.mDoors & DungeonRoom::DoorDirrection::DOWN)
				{
					mRooms[i + mWidth].mDoorGOs[DungeonRoom::DoorDirrection::UP]
						= room.mDoorGOs[DungeonRoom::DoorDirrection::DOWN];
				}

			}
		}
	}
}
