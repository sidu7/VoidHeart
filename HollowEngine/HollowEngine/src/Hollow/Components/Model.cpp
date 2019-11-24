#include <hollowpch.h>
#include "Model.h"

#include "Hollow/Managers/ResourceManager.h"

#include "Hollow/Components/Transform.h"

namespace Hollow {

	Model Model::instance;

	void Model::Init()
	{
		mCastShadow = false;
		mModelHasOffset = false;
		mModelPath = "";
		mShape = -1;
	}
	
	void Model::Clear()
	{
		mMeshes.clear();
	}
	
	void Model::Serialize(rapidjson::Value::Object object)
	{
		if (object.HasMember("Model"))
		{
			mModelPath = object["Model"].GetString();
			mMeshes = ResourceManager::Instance().LoadModel(mModelPath);
		}
		if(object.HasMember("ModelHasOffset"))
		{
			mModelHasOffset = object["ModelHasOffset"].GetBool();
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
		JSONHelper::Write<unsigned int>("Shape", mShape, writer);
		JSONHelper::Write<std::string>("Model", mModelPath, writer);
		JSONHelper::Write<bool>("CastShadow", mCastShadow, writer);
		JSONHelper::Write<bool>("ModelHasOffset", mModelHasOffset, writer);
	}

	void Model::DebugDisplay()
	{
	}
}
