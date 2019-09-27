#include <hollowpch.h>
#include "Material.h"

#include "Hollow/Managers/ResourceManager.h"

namespace Hollow {

	Material Material::instance;

	void Material::Init()
	{
	}
	Material::~Material()
	{
	}
	void Material::Serialize(rapidjson::Value::Object data)
	{
		rapidjson::Value::Array values = data["Diffuse"].GetArray();
		mDiffuseColor = glm::vec3(values[0].GetFloat(), values[1].GetFloat(), values[2].GetFloat());
		values = data["Specular"].GetArray();
		mSpecularColor = glm::vec3(values[0].GetFloat(), values[1].GetFloat(), values[2].GetFloat());
		mShininess = data["Shininess"].GetFloat();
		std::string path = data["Texture"].GetString();
		mpTexture = ResourceManager::Instance().LoadTexture(path);
	}
	void Material::DebugDisplay()
	{
		if (ImGui::TreeNode("Material"))
		{
			// Show the diffuse color
			ImGui::ColorEdit3("Diffuse", &mDiffuseColor[0], ImGuiColorEditFlags_Float);

			// Show the specular color
			ImGui::ColorEdit3("Specular", &mSpecularColor[0], ImGuiColorEditFlags_Float);

			// Show the shininess
			ImGui::InputFloat("Shininess", &mShininess);

			// Show the texture ID value
			//ImGui::InputScalar("Texture", ImGuiDataType_U32, &mTexture);

			ImGui::TreePop();
		}
	}
}