#pragma once
#include "Component.h"

namespace Hollow {
	class HOLLOW_API Material : public Component
	{
		REGISTERCOMPONENT(Material, "Material");
	public:
		void Init();

		void Clear() {}

		virtual ~Material();

		void DebugDisplay();

	public:
		glm::vec3 mDiffuseColor;
		glm::vec3 mSpecularColor;
		float mShininess;
		//unsigned int mTexture;
	};
}
