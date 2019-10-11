#pragma once
#include "Component.h"
namespace Hollow {
	class HOLLOW_API CameraController : public Component
	{
		REGISTERCOMPONENT(CameraController);
	public:
		void Init();
		void Serialize(rapidjson::Value::Object data);
		void Clear();
		void DebugDisplay();

	public:
		glm::vec3 mPosition;
		glm::vec3 mFront;
		glm::vec3 mUp;
		glm::vec3 mRight;
		glm::vec3 mWorldUp;

		float mYaw;
		float mPitch;

		float mMovementSpeed;
		float mMouseSensitivity;
		float mZoom;
		float mNear;
		float mFar;

		bool mCanMouse;

		float YAW;
		float PITCH;
		float SPEED;
		float SENSITIVITY;
		float ZOOM;
	};
}