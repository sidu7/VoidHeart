#include <hollowpch.h>
#include "Model.h"
#include "Hollow/Managers/ResourceManager.h"
#include "Hollow/Graphics/Mesh.h"

namespace Hollow {

	Model Model::instance;

	void Model::Init()
	{
		mCastShadow = false;
	}
	void Model::Clear()
	{
		mCastShadow = false;
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
		if (object.HasMember("CastShadow"))
		{
			mCastShadow = object["CastShadow"].GetBool();
		}
	}
	void Model::DebugDisplay()
	{
	}

}