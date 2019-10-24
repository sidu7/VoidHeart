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
		CameraProjection mProjection;
		glm::vec3 mPosition;
		glm::vec3 mFront;
		glm::vec3 mUp;			//Camera's Up Direction
		glm::vec3 mRight;
		glm::vec3 mWorldUp;		//World's Up Direction always (0.0f,1.0f,0.0f)

		float mYaw;
		float mPitch;

		float mMovementSpeed;
		float mMouseSensitivity;
		float mZoom;
		float mNearPlane;
		float mFarPlane;
		glm::ivec2 mScreenPosition;
		glm::ivec2 mViewPort;

		bool mCanMouse;

		float mDefaultZoom;		//Used to reset the values
		float mDefaultPitch;	//Used to reset the values
		float mDefaultYaw;		//Used to reset the values
	};
}