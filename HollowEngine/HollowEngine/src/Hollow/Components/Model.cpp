#include <hollowpch.h>
#include "Model.h"

#include "Hollow/Managers/ResourceManager.h"

#include "Hollow/Components/Transform.h"
#include "Hollow/Utils/ImGuiHelper.h"

namespace Hollow {

	Model Model::instance;

	void Model::Init()
	{
		mCastShadow = false;
		mModelHasOffset = false;
		mModelPath = "";
		mShape = SHAPE_NUM;
		mShapeType = "";
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
			mShapeType = object["Shape"].GetString();
			mShape = ResourceManager::Instance().mShapeEnumMap[mShapeType];
			mMeshes.push_back(ResourceManager::Instance().GetShape(mShapeType));
		}
		if (object.HasMember("CastShadow"))
		{
			mCastShadow = object["CastShadow"].GetBool();
		}
	}

	void Model::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		JSONHelper::Write("Shape", mShapeType, writer);
		JSONHelper::Write("Model", mModelPath, writer);
		JSONHelper::Write("CastShadow", mCastShadow, writer);
		JSONHelper::Write("ModelHasOffset", mModelHasOffset, writer);
	}

	void Model::DebugDisplay()
	{
		ImGuiHelper::InputText("Shape", mShapeType);
		ImGuiHelper::InputText("Model File", mModelPath);
		ImGui::Checkbox("CastShadow", &mCastShadow);
		ImGui::Checkbox("ModelHasOffset", &mModelHasOffset);
	}
}
