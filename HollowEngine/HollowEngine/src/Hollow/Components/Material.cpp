#include <hollowpch.h>
#include "Material.h"

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
		mDiffuseColor = glm::vec3(1.0, 0.0, 0.0);
		mSpecularColor = glm::vec3(1.0, 1.0, 1.0);
		mShininess = 10.0f;
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