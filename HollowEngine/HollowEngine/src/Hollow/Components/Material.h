#pragma once
#include "Component.h"

namespace Hollow {
	class Texture;
		
	class HOLLOW_API Material : public Component
	{
		REGISTERCOMPONENT(Material);
	public:
		void Init();

		void Clear() {}
		
		void Serialize(rapidjson::Value::Object data);

		void DebugDisplay();

	public:
		glm::vec3 mDiffuseColor;
		glm::vec3 mSpecularColor;
		float mShininess;
		Texture* mpTexture;		
		std::vector<MaterialData>
	};
}
