#pragma once
#include "Component.h"

namespace Hollow {
	class HOLLOW_API Script : public Component
	{
		REGISTERCOMPONENT(Script);
	public:
		void Init() override;
		void DebugDisplay() override;
		void Clear() override;
		void Serialize(rapidjson::Value::Object data) override;

		std::string scriptPath;
	};
}
