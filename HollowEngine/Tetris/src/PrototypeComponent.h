#pragma once
#include "Hollow.h"
#include "Hollow/Components/Component.h"

class PrototypeComponent : public Hollow::Component
{
	REGISTERCOMPONENT(PrototypeComponent);
public:
	void Init();
	void Serialize(rapidjson::Value::Object data) {}
	void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override{}
	void Clear();
	void DebugDisplay();

};