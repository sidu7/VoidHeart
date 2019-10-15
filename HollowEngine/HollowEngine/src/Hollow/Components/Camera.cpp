#include "hollowpch.h"
#include "Camera.h"

namespace Hollow {
	Camera Camera::instance;

	void Camera::Init()
	{
		mPosition = glm::vec3(0.0f, 0.0f, 0.0f);
		mFront = glm::vec3(0.0f, 0.0f, 0.0f);
		mWorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		mUp = mWorldUp;
		mRight = glm::vec3(0.0f, 0.0f, 0.0f);

		mYaw = -90.0f;
		mPitch = 0.0f;

		mMovementSpeed = 5.0f;
		mMouseSensitivity = 0.1f;
		mZoom = 45.0f;

		mNear = 0.1f;
		mFar = 1000.0f;

		mCanMouse = false;

		mDefaultZoom = mZoom;
		mDefaultPitch = mPitch;
		mDefaultYaw = mYaw;
	}
	void Camera::Serialize(rapidjson::Value::Object data)
	{

		if (data.HasMember("Position"))
		{
			mPosition = JSONHelper::GetVec3F(data["Position"].GetArray());
		}

		if (data.HasMember("Yaw"))
		{
			mYaw = data["Yaw"].GetFloat();
			mDefaultYaw = mYaw;
		}
		if (data.HasMember("Pitch"))
		{
			mPitch = data["Pitch"].GetFloat();
			mDefaultPitch = mPitch;
		}
		if (data.HasMember("MovementSpeed"))		//TODO: Change the name??
		{
			mMovementSpeed = data["MovementSpeed"].GetFloat();
		}

		if (data.HasMember("MouseSensitivity"))
		{
			mMouseSensitivity = data["MouseSensitivity"].GetFloat();
		}

		if (data.HasMember("Zoom"))
		{
			mZoom = data["Zoom"].GetFloat();
			mDefaultZoom = mZoom;
		}

		if (data.HasMember("Near"))					//TODO: Change the name?
		{
			mNear = data["Near"].GetFloat();
		}

		if (data.HasMember("Far"))					//TODO: Change the name?
		{
			mFar = data["Far"].GetFloat();
		}


	}
	void Camera::Clear()
	{
		mPosition = glm::vec3(0.0f, 0.0f, 0.0f);
		mFront = glm::vec3(0.0f, 0.0f, 0.0f);
		mWorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		mUp = mWorldUp;
		mRight = glm::vec3(0.0f, 0.0f, 0.0f);

		mYaw = -90.0f;
		mPitch = 0.0f;

		mMovementSpeed = 5.0f;
		mMouseSensitivity = 0.1f;
		mZoom = 45.0f;

		mNear = 0.1f;
		mFar = 1000.0f;

		mCanMouse = false;

		mDefaultZoom = mZoom;
		mDefaultPitch = mPitch;
		mDefaultYaw = mYaw;
	}
	void Camera::DebugDisplay()
	{
		if (ImGui::TreeNode("Camera"))
		{
			ImGui::InputFloat3("Position", &mPosition[0]);
			ImGui::InputFloat("Zoom", &mZoom);
			ImGui::InputFloat("Yaw", &mYaw);
			ImGui::InputFloat("Pitch", &mPitch);
			ImGui::InputFloat("Mouse Sensitivity", &mMouseSensitivity);
			ImGui::TreePop();
		}
	}
}
