#pragma once
#include "Hollow.h"
#include "Hollow/Components/Component.h"

namespace BulletHell
{
	class DestroyTimer : public Hollow::Component
	{
		REGISTERCOMPONENT(DestroyTimer);
	public:
		/// <summary>
		/// Initializes the DestroyTimer component.
		/// </summary>
		void Init();

		/// <summary>
		/// Serializes the specified data of the DestroyTimer component from the JSON file.
		/// </summary>
		/// <param name="data">JSON object.</param>
		void Serialize(rapidjson::Value::Object data);

		/// <summary>
		/// DeSerialize the current DestroyTimer component into a JSON file.
		/// </summary>
		/// <param name="writer">The writer of the JSON file.</param>
		void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer);

		/// <summary>
		/// Clears the DestroyTimer component.
		/// </summary>
		void Clear();

		/// <summary>
		/// To set the Debug Display for the DestroyTimer Component.
		/// </summary>
		void DebugDisplay();

	public:
		float mMaxTime;
		float mCurrentTime;
	};
}