#include <hollowpch.h>
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

		mOffsetFromAnchor = glm::vec3(0.0f);
		
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

		if (data.HasMember("Near"))
		{
			mNearPlane = data["Near"].GetFloat();
		}

		if (data.HasMember("Far"))
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
			 mDViewPortPosition = JSONHelper::GetIVec2(data["ViewPortPosition"].GetArray());
			 glm::vec2 size = RenderManager::Instance().GetWindowSize();
			 mViewPortPosition = glm::vec2(mDViewPortPosition.x < 0 ?
				 size.x + mDViewPortPosition.x : mDViewPortPosition.x, mDViewPortPosition.y < 0 ?
				 size.y + mDViewPortPosition.y : mDViewPortPosition.y);
		}
		if (data.HasMember("ViewPortSize"))
		{
			mViewPortSize = JSONHelper::GetIVec2(data["ViewPortSize"].GetArray());
		}
		if (data.HasMember("ProjectionType"))
		{
			mProjectionType = (CameraProjection)data["ProjectionType"].GetUint();
		}
		if (data.HasMember("PositionOffset"))
		{
			mOffsetFromAnchor = JSONHelper::GetVec3F(data["PositionOffset"].GetArray());
		}
		if (data.HasMember("AnchorFocusOffset"))
		{
			mAnchorFocusOffset = JSONHelper::GetVec3F(data["AnchorFocusOffset"].GetArray());
		}
		
	}
	void Camera::Clear()
	{

	}
	void Camera::DebugDisplay()
	{
		ImGui::InputFloat("Yaw", &mYaw);
		ImGui::InputFloat("Pitch", &mPitch);
		ImGui::InputFloat("MovementSpeed", &mMovementSpeed);
		ImGui::InputFloat("MouseSensitivity", &mMouseSensitivity);
		ImGui::InputFloat("Zoom", &mZoom);
		ImGui::InputFloat("Near", &mNearPlane);
		ImGui::InputFloat("Far", &mFarPlane);
		ImGui::Checkbox("Active", &mIsActive);
		ImGui::InputInt("CameraType", (int*)&mType);
		ImGui::InputInt("ProjectionType", (int*)&mProjectionType);
		ImGui::InputInt2("ViewPortPosition", &mDViewPortPosition[0]);
		ImGui::InputInt2("ViewPortSize", &mViewPortSize[0]);
		ImGui::InputFloat3("PositionOffset", &mOffsetFromAnchor[0]);
	}

	void Camera::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		JSONHelper::Write("Yaw", mYaw, writer);
		JSONHelper::Write("Pitch", mPitch, writer);
		JSONHelper::Write("MovementSpeed", mMovementSpeed, writer);
		JSONHelper::Write("MouseSensitivity", mMouseSensitivity, writer);
		JSONHelper::Write("Zoom", mZoom, writer);
		JSONHelper::Write("Near", mNearPlane, writer);
		JSONHelper::Write("Far", mFarPlane, writer);
		JSONHelper::Write("IsActive", mIsActive, writer);
		JSONHelper::Write("CameraType", (unsigned)mType, writer);
		JSONHelper::Write("ProjectionType", (unsigned)mProjectionType, writer);
		JSONHelper::Write("ViewPortPosition", mDViewPortPosition, writer);
		JSONHelper::Write("ViewPortSize", mViewPortSize, writer);
		JSONHelper::Write("PositionOffset", mOffsetFromAnchor, writer);		
		
	}
}
