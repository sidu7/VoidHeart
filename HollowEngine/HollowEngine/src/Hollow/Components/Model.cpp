#include <hollowpch.h>
#include "Model.h"
#include "Hollow/Managers/ResourceManager.h"
#include "Hollow/Graphics/Mesh.h"

namespace Hollow {

	Model Model::instance;

	void Model::Init()
	{

	}
	void Model::Clear()
	{
	}
	void Model::Serialize(rapidjson::Value::Object object)
	{
		if (object.HasMember("Model"))
		{
			mMeshes = ResourceManager::Instance().LoadModel(object["Model"].GetString());
		}
		if (object.HasMember("Shape"))
		{
			mMeshes.push_back(ResourceManager::Instance().GetShape((Shapes)object["Shape"].GetUint()));
		}
	}
	void Model::DebugDisplay()
	{
	}

}