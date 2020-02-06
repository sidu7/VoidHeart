#pragma once
#include "Hollow.h"
#include "Hollow/Components/Component.h"

namespace BulletHell
{
    class AISetting : public Hollow::Component
    {
        REGISTERCOMPONENT(AISetting);
    public:
        /// <summary>
        /// Initializes the Attack component.
        /// </summary>
        void Init();

        /// <summary>
        /// Serializes the specified data of the Attack component from the JSON file.
        /// </summary>
        /// <param name="data">JSON object.</param>
        void Serialize(rapidjson::Value::Object data);

        /// <summary>
        /// DeSerialize the current Attack component into a JSON file.
        /// </summary>
        /// <param name="writer">The writer of the JSON file.</param>
        void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer);

        /// <summary>
        /// Clears the Attack component.
        /// </summary>
        void Clear();

        /// <summary>
        /// To set the Debug Display for the Attack Component.
        /// </summary>
        void DebugDisplay();

    public:
        std::string mMovementScriptPath;
        std::string mShootingScriptPath;

        int mFloorNum;
        int mRoomIndex; // index into 2D array in the DungeonFloor vector of rooms
    };
}