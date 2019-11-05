#include "hollowpch.h"
#include "Camera.h"

#include "Hollow/Graphics/Data/CameraData.h"

#include "Hollow/Managers/RenderManager.h"

namespace Hollow {
	Camera Camera::instance;

	void Camera::Init()
	{
		mFront = glm::vec3(0.0f, 0.0f, 0.0f);
		mUp = glm::vec3(0.0f, 1.0f, 0.0f);
		mRight = glm::vec3(0.0f, 0.0f, 0.0f);

		mYaw = -90.0f;
		mPitch = 0.0f;

		mIsActive = false;
		
		mMovementSpeed = 5.0f;
		mMouseSensitivity = 0.1f;
		mZoom = 45.0f;

		mNearPlane = 0.1f;
		mFarPlane = 1000.0f;
		mAspectRatio = 1.0f;
		
		mDefaultZoom = mZoom;
		mDefaultPitch = mPitch;
		mDefaultYaw = mYaw;

		mViewPortPosition = glm::ivec2(0,0);
		mViewPortSize = glm::ivec2(0, 0);
	}
	void Camera::Serialize(rapidjson::Value::Object data)
	{

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
		if (data.HasMember("IsActive"))
		{
			mIsActive = data["IsActive"].GetBool();
		}
		if (data.HasMember("CameraType"))
		{
			mType = (CameraType)data["CameraType"].GetUint();
		}
		if (data.HasMember("ViewPortPosition"))
		{
			 glm::vec2 pos = JSONHelper::GetIVec2(data["ViewPortPosition"].GetArray());
			 glm::vec2 size = RenderManager::Instance().GetWindowSize();
			 mViewPortPosition = glm::vec2(pos.x < 0 ? size.x + pos.x : pos.x, pos.y < 0 ? size.y + pos.y : pos.y);
		}
		if (data.HasMember("ViewPortSize"))
		{
			mViewPortSize = JSONHelper::GetIVec2(data["ViewPortSize"].GetArray());
		}
		if (data.HasMember("ProjectionType"))
		{
			mProjectionType = (CameraProjection)data["ProjectionType"].GetUint();
		}
		
	}
	void Camera::Clear()
	{

	}
	void Camera::DebugDisplay()
	{
		if (ImGui::TreeNode("Camera"))
		{
			ImGui::InputFloat("Zoom", &mZoom);
			ImGui::InputFloat("Yaw", &mYaw);
			ImGui::InputFloat("Pitch", &mPitch);
			ImGui::InputFloat("Mouse Sensitivity", &mMouseSensitivity);
			ImGui::TreePop();
		}
	}
}
