#pragma once

//#include "Hollow/Common.h"
#include "DungeonFloor.h"
#include "Hollow/Utils/Singleton.h"

namespace BulletHell
{
    class DungeonManager
    {
    private: 
        DungeonManager() : mSeed(0) { std::cout << "Initialized: DungeonManager" << std::endl; }			
    public: 
        DungeonManager(const DungeonManager&) = delete;
        DungeonManager& operator= (const DungeonManager&) = delete;
        DungeonManager(DungeonManager&&) = delete;
        DungeonManager& operator=(DungeonManager&&) = delete;
        static DungeonManager& Instance()
        {									
            static DungeonManager instance;
            return instance;				
        }

    public:
        void Init();
        void Generate();
        void Regenerate();

        std::vector<DungeonFloor> GetFloors() const;
        void Print() const;
    private:

    public:

    private:
        std::vector<DungeonFloor> mFloors;
        unsigned mSeed;
    };
}
