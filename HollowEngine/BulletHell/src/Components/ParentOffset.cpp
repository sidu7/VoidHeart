#include "ParentOffset.h"

#include "Hollow/Managers/GameObjectManager.h"

namespace BulletHell
{
	ParentOffset ParentOffset::instance;

	void ParentOffset::Init()
	{
	}

	void ParentOffset::Serialize(rapidjson::Value::Object data)
	{
		if (data.HasMember("Parent"))
		{
			int parentType = data["Parent"].GetInt();
			std::vector<Hollow::GameObject*> possibleParents = Hollow::GameObjectManager::Instance().GetObjectByType(parentType);
			if (possibleParents.empty())
			{
				mpParent = nullptr;
			}
			else
			{
				mpParent = possibleParents[0];
			}
		}
		if (data.HasMember("Offset"))
		{
			mOffset = Hollow::JSONHelper::GetVec3F(data["Offset"].GetArray());
		}
	}

	void ParentOffset::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		Hollow::JSONHelper::Write<glm::vec3>("Offset", mOffset, writer);
	}

	void ParentOffset::Clear()
	{
	}

	void ParentOffset::DebugDisplay()
	{
		ImGui::Text("%i", mpOwner->mType);
		ImGui::InputFloat3("Offset", &mOffset[0]);
	}
}