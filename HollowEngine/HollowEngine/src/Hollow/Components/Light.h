#pragma once
#include "Component.h"

namespace Hollow
{
	class FrameBuffer;

	enum LightType
	{
		GLOBAL,
		LOCAL,
		LIGHT_NUM
	};

	class Light : public Component
	{
		REGISTERCOMPONENT(Light);
	public:
		/// <summary>
		/// Initializes the Light component.
		/// </summary>
		HOLLOW_API void Init() override;

		/// <summary>
		/// Serializes the specified data of the Light component from the JSON file.
		/// </summary>
		/// <param name="data">JSON object.</param>
		HOLLOW_API void Serialize(rapidjson::Value::Object data) override;

		/// <summary>
		/// DeSerialize the current Light component into a JSON file.
		/// </summary>
		/// <param name="writer">The writer of the JSON file.</param>
		HOLLOW_API void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;

		/// <summary>
		/// Clears the Light component.
		/// </summary>
		HOLLOW_API void Clear() override;

		/// <summary>
		/// To set the Debug Display for the Light Component.
		/// </summary>
		HOLLOW_API void DebugDisplay() override;

	public:
		glm::vec3 mPosition; // Position of light or offset from the objects center position ?
		float mRadius;
		glm::vec3 mColor;
		glm::vec3 mLookAtPoint;
		LightType mType;

		// Shadow information
		bool mCastShadow;
		FrameBuffer* mpShadowMap;
		glm::vec2 mShadowMapSize;
		float mShadowMapNearPlane;
		float mShadowMapFarPlane;
		float mFOV;

		// Moment shadow map information
		unsigned int mBlurDistance;
		float mAlpha;
		float mMD; // Hopefully can find a good constant or formula to get this
	};
}