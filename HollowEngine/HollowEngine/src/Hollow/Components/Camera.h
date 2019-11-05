#pragma once
#include "Component.h"

namespace Hollow 
{
	enum CameraType;
	enum CameraProjection;

	class Camera : public Component
	{
		REGISTERCOMPONENT(Camera);
	public:
		void Init();
		void Serialize(rapidjson::Value::Object data);
		void Clear();
		void DebugDisplay();

	public:
		CameraType mType;
		CameraProjection mProjectionType;
		glm::vec3 mFront;
		glm::vec3 mUp;			//Camera's Up Direction
		glm::vec3 mRight;

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
	};
}