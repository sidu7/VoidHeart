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
			mModelPath = object["Model"].GetString();
			mMeshes = ResourceManager::Instance().LoadModel(mModelPath);
		}
		if (object.HasMember("Shape"))
		{
			mShape = object["Shape"].GetUint();
			mMeshes.push_back(ResourceManager::Instance().GetShape((Shapes)mShape));
		}
		if (object.HasMember("CastShadow"))
		{
			mCastShadow = object["CastShadow"].GetBool();
		}
	}

	void Model::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		if (mMeshes.size() == 1)
		{
			JSONHelper::Write<unsigned int>("Shape", mShape, writer);
		}
		else
		{
			JSONHelper::Write<std::string>("Model", mModelPath, writer);
		}
		JSONHelper::Write<bool>("CastShadow", mCastShadow, writer);
	}

	void Model::DebugDisplay()
	{
	}

}