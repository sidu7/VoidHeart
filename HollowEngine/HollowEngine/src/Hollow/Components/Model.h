#pragma once
#include "Component.h"

namespace Hollow
{
	class Mesh;
	class Shader;

	class HOLLOW_API Model : public Component
	{
		REGISTERCOMPONENT(Model,"Model")
	public:
		void Init();
		void Clear();
		void Serialize(rapidjson::Value::Object object);
		void DebugDisplay();
		inline std::vector<Mesh*>& GetMeshes() { return mMeshes; }

	private:
		std::vector<Mesh*> mMeshes;
		Shader* shader;
	};
}