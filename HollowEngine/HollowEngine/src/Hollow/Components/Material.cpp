#include <hollowpch.h>
#include "Material.h"

#include "Hollow/Managers/ResourceManager.h"

namespace Hollow {

	Material Material::instance;

	void Material::Init()
	{
		mpTexture = nullptr;
	}

	void Material::Serialize(rapidjson::Value::Object data)
	{		
		if (data.HasMember("Texture"))
		{
			std::string path = data["Texture"].GetString();
			mpTexture = ResourceManager::Instance().LoadTexture(path);
		}
		if (data.HasMember("Diffuse"))
		{
			mDiffuseColor = JSONHelper::GetVec3F(data["Diffuse"].GetArray());
		}
		if (data.HasMember("Specular"))
		{
			mSpecularColor = JSONHelper::GetVec3F(data["Specular"].GetArray());
		}
		if (data.HasMember("Shininess"))
		{
			mShininess = data["Shininess"].GetFloat();
		}
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