#pragma once
#include "rapidjson/document.h"
#include "glm/glm.hpp"

class JSONHelper
{
public:
	inline static glm::vec3 GetVec3F(rapidjson::Value::Array arr)
	{
		return glm::vec3(arr[0].GetFloat(), arr[1].GetFloat(), arr[2].GetFloat());
	}

	inline static glm::vec2 GetVec2F(rapidjson::Value::Array arr)
	{
		return glm::vec2(arr[0].GetFloat(), arr[1].GetFloat());
	}
};