#pragma once
#include "Component.h"

namespace Hollow {
	class Script : public Component
	{
		REGISTERCOMPONENT(Script);
	public:
		/// <summary>
		/// Initializes the Script component.
		/// </summary>
		HOLLOW_API void Init() override;

		/// <summary>
		/// To set the Debug Display for the Script Component.
		/// </summary>
		HOLLOW_API void DebugDisplay() override;

		/// <summary>
		/// Clears the Script component.
		/// </summary>
		HOLLOW_API void Clear() override;

		/// <summary>
		/// Serializes the specified data of the Script component from the JSON file.
		/// </summary>
		/// <param name="data">JSON object.</param>
		HOLLOW_API void Serialize(rapidjson::Value::Object data) override;

		/// <summary>
		/// DeSerialize the current Script component into a JSON file.
		/// </summary>
		/// <param name="writer">The writer of the JSON file.</param>
		HOLLOW_API void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;

		std::vector <std::string> mScripts;
		std::vector <std::string> mDestroyScripts;

		// Active
		bool mIsActive;
	private:
		int mScriptCount;
		int mDestroyScriptCount;
	};
}
