#include "hollowpch.h"
#include "Camera.h"

#include "Hollow/Graphics/Data/CameraData.h"

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

		mNearPlane = 0.1f;
		mFarPlane = 1000.0f;

		mCanMouse = false;

		mDefaultZoom = mZoom;
		mDefaultPitch = mPitch;
		mDefaultYaw = mYaw;

		mScreenPosition = glm::ivec2(0,0);
		mViewPort = glm::ivec2(0, 0);
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

		if (data.HasMember("NearPlane"))
		{
			mNearPlane = data["Near"].GetFloat();
		}

		if (data.HasMember("FarPlane"))
		{
			mFarPlane = data["Far"].GetFloat();
		}

		if (data.HasMember("CameraType"))
		{
			mType = (CameraType)data["CameraType"].GetUint();
		}
		if (data.HasMember("Projection"))
		{
			mProjection = (CameraProjection)data["Projection"].GetUint();
		}
		if (data.HasMember("ScreenPosition"))
		{
			mScreenPosition = JSONHelper::GetIVec2(data["ScreenPosition"].GetArray());
		}
		if (data.HasMember("ScreenViewPort"))
		{
			mViewPort = JSONHelper::GetIVec2(data["ScreenViewPort"].GetArray());
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

		mNearPlane = 0.1f;
		mFarPlane = 1000.0f;

		mCanMouse = false;

		mDefaultZoom = mZoom;
		mDefaultPitch = mPitch;
		mDefaultYaw = mYaw;

		mScreenPosition = glm::ivec2(0, 0);
		mViewPort = glm::ivec2(0, 0);
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
