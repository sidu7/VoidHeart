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
		//if Shape get from Resource Manager or use Mesh data from file
		//mMeshes.push_back(ResourceManager::Instance().GetShape(Shapes::TEAPOT));
		mMeshes = ResourceManager::Instance().LoadModel(object["Model"].GetString());
	}
	void Model::DebugDisplay()
	{
	}

}