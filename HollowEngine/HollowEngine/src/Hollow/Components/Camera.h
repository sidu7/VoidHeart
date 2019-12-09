#pragma once
#include "Component.h"

namespace Hollow 
{
	enum CameraType;
	enum CameraProjection;
	class Transform;
	class Camera : public Component
	{
		REGISTERCOMPONENT(Camera);
	public:
		/// <summary>
		/// Initializes the Camera component.
		/// </summary>
		HOLLOW_API void Init();

		/// <summary>
		/// Serializes the specified data of the Camera component from the JSON file.
		/// </summary>
		/// <param name="data">JSON object.</param>
		HOLLOW_API void Serialize(rapidjson::Value::Object data);

		/// <summary>
		/// Clears the Camera component.
		/// </summary>
		HOLLOW_API void Clear();

		/// <summary>
		/// To set the Debug Display for the Camera Component.
		/// </summary>
		HOLLOW_API void DebugDisplay();

		/// <summary>
		/// DeSerialize the current Camera component into a JSON file.
		/// </summary>
		/// <param name="writer">The writer of the JSON file.</param>
		HOLLOW_API void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;

	public:
		CameraType mType;
		CameraProjection mProjectionType;
		glm::vec3 mFront;
		glm::vec3 mUp;			//Camera's Up Direction
		glm::vec3 mRight;

		glm::vec3 mOffsetFromAnchor;

		// Top down camera variables
		glm::vec3 mPreviousPosition;
		float mLERPFactor;
		glm::vec2 mXConstraints;
		glm::vec2 mYConstraints;
		glm::vec2 mZConstraints;
		
		glm::vec3 mAnchorFocusOffset;

		bool mIsActive;
		
		float mYaw;
		float mPitch;
		float mAspectRatio;
		
		float mMovementSpeed;
		float mMouseSensitivity;
		float mZoom;
		float mNearPlane;
		float mFarPlane;
		glm::ivec2 mViewPortPosition;
		glm::ivec2 mViewPortSize;

		float mDefaultZoom;		//Used to reset the values
		float mDefaultPitch;	//Used to reset the values
		float mDefaultYaw;		//Used to reset the values

		std::vector <Transform*> mFocusPositions;
		std::vector <std::string> mFocusObjects;

		float mPitchLowerBound;
		
	private:
		glm::ivec2 mDViewPortPosition;
	};
}