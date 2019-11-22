#pragma once
#include "Hollow/Core/Core.h"
#include "Hollow/Utils/Log.h"
#include "rapidjson/document.h"
#include "glm/glm.hpp"

#ifdef _MSC_VER   
#undef GetObject
#endif

namespace Hollow
{
	class JSONHelper
	{
	public:

#define PARSE_JSON_FILE(x) std::ifstream file(x);\
		std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());\
		file.close(); \
		rapidjson::Document root;\
		root.Parse(contents.c_str());\
		if(!root.IsObject()) { HW_CORE_ERROR("Error reading JSON file {0}",x); }

		//JSON parsing
		HOLLOW_API static rapidjson::Value::Object GetSettings(rapidjson::Value::Object& data, const char* setting)
		{
			if (!data.HasMember(setting))
			{
				HW_CORE_ERROR("Setting {0} not found", setting);
			}
			return data[setting].GetArray()[0].GetObject();
		}

		HOLLOW_API static glm::vec3 GetVec3F(const rapidjson::Value::Array& arr)
		{
			return glm::vec3(arr[0].GetFloat(), arr[1].GetFloat(), arr[2].GetFloat());
		}

		HOLLOW_API static glm::vec2 GetVec2F(const rapidjson::Value::Array& arr)
		{
			return glm::vec2(arr[0].GetFloat(), arr[1].GetFloat());
		}

		HOLLOW_API static glm::vec4 GetVec4F(const rapidjson::Value::Array& arr)
		{
			return glm::vec4(arr[0].GetFloat(), arr[1].GetFloat(), arr[2].GetFloat(), arr[3].GetFloat());
		}

		HOLLOW_API static glm::ivec2 GetIVec2(const rapidjson::Value::Array& arr)
		{
			return glm::ivec2(arr[0].GetInt(), arr[1].GetInt());
		}

		template<typename T>
		static std::vector<T> GetArray(const rapidjson::Value::Array& arr)
		{
			std::vector<T> data;
			data.reserve(arr.Size());
			for (unsigned int i = 0; i < arr.Size(); ++i)
			{
				data.emplace_back(static_cast<T>(arr[i].GetUint()));
			}
			return data;
		}

		template<>
		HOLLOW_API static std::vector<std::string> GetArray(const rapidjson::Value::Array& arr)
		{
			std::vector<std::string> data;
			data.reserve(arr.Size());
			for (unsigned int i = 0; i < arr.Size(); ++i)
			{
				data.emplace_back(arr[i].GetString());
			}
			return data;
		}

		template<>
		HOLLOW_API static std::vector<unsigned int> GetArray(const rapidjson::Value::Array& arr)
		{
			std::vector<unsigned int> data;
			data.reserve(arr.Size());
			for (unsigned int i = 0; i < arr.Size(); ++i)
			{
				data.emplace_back(arr[i].GetUint());
			}
			return data;
		}

		//JSON deserialization
		template<typename T>
		static void Write(const char* key, const T& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
		{
			HW_CORE_ERROR("No specialized template for this value type");
		}

		template<>
		HOLLOW_API static void Write(const char* key, const int& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
		{
			writer.Key(key);
			writer.Int(value);
		}

		template<>
		HOLLOW_API static void Write(const char* key, const unsigned int& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
		{
			writer.Key(key);
			writer.Uint(value);
		}

		template<>
		HOLLOW_API static void Write(const char* key, const bool& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
		{
			writer.Key(key);
			writer.Bool(value);
		}

		template<>
		HOLLOW_API static void Write(const char* key, const float& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
		{
			writer.Key(key);
			writer.Double(value);
		}

		template<>
		HOLLOW_API static void Write(const char* key, const double& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
		{
			writer.Key(key);
			writer.Double(value);
		}

		template<>
		HOLLOW_API static void Write(const char* key, const std::string& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
		{
			writer.Key(key);
			writer.String(value.c_str());
		}

		template<>
		HOLLOW_API static void Write(const char* key, const glm::vec4& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
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
		HOLLOW_API static void Write(const char* key, const glm::vec3& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
		{
			writer.Key(key);
			writer.StartArray();
			writer.Double(value.x);
			writer.Double(value.y);
			writer.Double(value.z);
			writer.EndArray();
		}

		template<>
		HOLLOW_API static void Write(const char* key, const glm::vec2& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
		{
			writer.Key(key);
			writer.StartArray();
			writer.Double(value.x);
			writer.Double(value.y);
			writer.EndArray();
		}
	};
}