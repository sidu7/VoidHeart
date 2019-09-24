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
	void Material::DebugDisplay()
	{
		if (ImGui::TreeNode("Material"))
		{
			// Show the diffuse color
			ImGui::ColorEdit3("Diffuse", &mDiffuseColor[0], ImGuiColorEditFlags_Float);

			// Show the specular color
			ImGui::ColorEdit3("Specular", &mSpecularColor[0], ImGuiColorEditFlags_Float);

			// Show the shininess
			// TODO: Fix this, check ImGui input capturing
			ImGui::InputFloat("Shininess", &mShininess);
			ImGui::SliderFloat("Shininess", &mShininess,0.0f, 1000.0f);

			// Show the texture ID value
			//ImGui::InputScalar("Texture", ImGuiDataType_U32, &mTexture);

			ImGui::TreePop();
		}
	}
}