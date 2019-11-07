#pragma once
#include "Component.h"

namespace Hollow {
	class Texture;
	struct MaterialData;
		
	class Material : public Component
	{
		REGISTERCOMPONENT(Material);
	public:
		HOLLOW_API void Init();
		HOLLOW_API void Clear();
		HOLLOW_API void Serialize(rapidjson::Value::Object data);
		HOLLOW_API void DebugDisplay();

	public:
		glm::vec3 mDiffuseColor;
		glm::vec3 mSpecularColor;
		float mShininess;
		float mHeightScale;
		Texture* mpTexture;
		std::vector<MaterialData*> mMaterials;
	};
}
