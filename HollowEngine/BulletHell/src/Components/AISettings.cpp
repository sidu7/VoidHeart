#include "AISettings.h"

#include "Hollow/Managers/ScriptingManager.h"

#include "Hollow/Components/Transform.h"

#include "Hollow/Managers/GameObjectManager.h"

namespace BulletHell
{
    AISetting AISetting::instance;

    void AISetting::Init()
    {
    }

    void AISetting::Serialize(rapidjson::Value::Object data)
    {
        if (data.HasMember("MovementScript"))
        {
            mMovementScriptPath = data["MovementScript"].GetString();
        }
        if (data.HasMember("ShootingScript"))
        {
            mShootingScriptPath = data["ShootingScript"].GetString();
        }
        if (data.HasMember("FloorNum"))
        {
            mFloorNum = data["FloorNum"].GetInt();
        }
        if (data.HasMember("RoomIndex"))
        {
            mRoomIndex = data["RoomIndex"].GetInt();
        }
    }

    void AISetting::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
    {
        Hollow::JSONHelper::Write<std::string>("MovementScript", mMovementScriptPath, writer);
        Hollow::JSONHelper::Write<std::string>("MovementScript", mShootingScriptPath, writer);
        Hollow::JSONHelper::Write<int>("FloorNum", mFloorNum, writer);
        Hollow::JSONHelper::Write<int>("RoomIndex", mRoomIndex, writer);
    }

    void AISetting::Clear()
    {
    }

    void AISetting::DebugDisplay()
    {
        char* str = (char*)mMovementScriptPath.c_str();
        ImGui::InputText("Movement Script Path", str, 50);
        mMovementScriptPath = str;
        str = (char*)mShootingScriptPath.c_str();
        ImGui::InputText("Shooting Script Path", str, 50);
        mShootingScriptPath = str;

        ImGui::Text("Floor Num: %f", mFloorNum);
        ImGui::Text("Room Index: %f", mRoomIndex);
    }
}