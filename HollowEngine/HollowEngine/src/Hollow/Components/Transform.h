#pragma once
#include "Component.h"

namespace Hollow
{
	class Transform : public Component
	{
		REGISTERCOMPONENT(Transform);
	public:

		/// <summary>
		/// Initializes the Transform component.
		/// </summary>
		HOLLOW_API void Init() override;

		/// <summary>
		/// Clears the Transform component.
		/// </summary>
		HOLLOW_API void Clear() override {}

		/// <summary>
		/// To set the Debug Display for the Transform Component.
		/// </summary>
		HOLLOW_API void DebugDisplay() override;

		/// <summary>
		/// Serializes the specified data of the Transform component from the JSON file.
		/// </summary>
		/// <param name="data">JSON object.</param>
		HOLLOW_API void Serialize(rapidjson::Value::Object data) override;

		/// <summary>
		/// DeSerialize the current Transform component into a JSON file.
		/// </summary>
		/// <param name="writer">The writer of the JSON file.</param>
		HOLLOW_API void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;

        /// <summary>
        /// Gets orientation vector.
        /// </summary>
        HOLLOW_API glm::vec3 GetForward() const;

        HOLLOW_API void Rotate(const glm::vec3 angles);

		glm::vec3 mPosition;					// mPosition is x,y,z
		glm::vec3 mScale;						// mScale is x,y,z
		glm::vec3 mRotation;					// mRotation is degree for x,y,z
		glm::mat4 mTransformationMatrix;		// Transformation Matrix
		glm::mat4 mDebugTransformationMatrix;	// Debug Transformation Matrix
		glm::mat4 mPreviousTransformationMatrix;// For PostProcessing effects
		glm::fquat mQuaternion;					// mQuaternion Do not mess if you don't know this
		
		bool dirtyBit;

	};
}