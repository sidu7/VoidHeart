#pragma once
#include "Hollow/Core/Core.h"
#include "rapidjson/document.h"
#include "glm/glm.hpp"

namespace Hollow
{
	class HOLLOW_API JSONHelper
	{
	public:
		inline static glm::vec3 GetVec3F(const rapidjson::Value::Array& arr)
		{
			return glm::vec3(arr[0].GetFloat(), arr[1].GetFloat(), arr[2].GetFloat());
		}

		inline static glm::vec2 GetVec2F(const rapidjson::Value::Array& arr)
		{
			return glm::vec2(arr[0].GetFloat(), arr[1].GetFloat());
		}

		inline static glm::vec4 GetVec4F(const rapidjson::Value::Array& arr)
		{
			return glm::vec4(arr[0].GetFloat(), arr[1].GetFloat(), arr[2].GetFloat(), arr[3].GetFloat());
		}

		template<typename T>
		inline static void Write(const char* key, const T& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
		{
			HW_CORE_ERROR("No specialized template for this value type");
		}

		template<>
		inline static void Write(const char* key, const int& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
		{
			writer.Key(key);
			writer.Int(value);
		}

		template<>
		inline static void Write(const char* key, const unsigned int& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
		{
			writer.Key(key);
			writer.Uint(value);
		}

		template<>
		inline static void Write(const char* key, const bool& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
		{
			writer.Key(key);
			writer.Bool(value);
		}

		template<>
		inline static void Write(const char* key, const float& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
		{
			writer.Key(key);
			writer.Double(value);
		}

		template<>
		inline static void Write(const char* key, const double& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
		{
			writer.Key(key);
			writer.Double(value);
		}

		template<>
		inline static void Write(const char* key, const std::string& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
		{
			writer.Key(key);
			writer.String(value.c_str());
		}

		template<>
		inline static void Write(const char* key, const glm::vec4& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
		{
			writer.Key(key);
			writer.StartArray();
			writer.Double(value.x);
			writer.Double(value.y);
			writer.Double(value.z);
			writer.Double(value.w);
			writer.EndArray();
		}

		template<>
		inline static void Write(const char* key, const glm::vec3& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
		{
			writer.Key(key);
			writer.StartArray();
			writer.Double(value.x);
			writer.Double(value.y);
			writer.Double(value.z);
			writer.EndArray();
		}

		template<>
		inline static void Write(const char* key, const glm::vec2& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
		{
			writer.Key(key);
			writer.StartArray();
			writer.Double(value.x);
			writer.Double(value.y);
			writer.EndArray();
		}
	};
}