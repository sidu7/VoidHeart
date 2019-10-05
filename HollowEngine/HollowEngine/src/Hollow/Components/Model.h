#pragma once
#include "Component.h"

namespace Hollow
{
	class Mesh;
	class Shader;

	class HOLLOW_API Model : public Component
	{
		REGISTERCOMPONENT(Model);
	public:
		void Init();
		void Clear();
		void Serialize(rapidjson::Value::Object object);
		void DebugDisplay();

		std::vector<Mesh*> mMeshes;
		Shader* mpShader;
		bool mCastShadow;
	};
}