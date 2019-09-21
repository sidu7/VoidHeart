#include <hollowpch.h>
#include "Camera.h"

namespace Hollow {

	const float Camera::YAW = -90.0f;
	const float Camera::PITCH = 0.0f;
	const float Camera::SPEED = 25.0f;
	const float Camera::SENSITIVITY = 0.05f;
	const float Camera::ZOOM = 45.0f;

	Camera::Camera(const glm::vec3& position,const glm::vec3& up, float yaw, float pitch) : mFront(glm::vec3(0.0f, 0.0f, -1.0f)), mMovementSpeed(SPEED), mMouseSensitivity(SENSITIVITY), mZoom(ZOOM)
	{
		mPosition = position;
		mWorldUp = up;
		mYaw = yaw;
		mPitch = pitch;
		UpdateCamera();
	}

	glm::mat4 Camera::GetViewMatrix()
	{
		return glm::lookAt(mPosition, mPosition + mFront, mUp);
	}

	void Camera::HandleKeyboardInput(int direction, float deltaTime)
	{
		float velocity = mMovementSpeed * deltaTime;
		if (direction == 0)
		{
			mPosition += mFront * velocity;
		}
		if (direction == 1)
		{
			mPosition -= mFront * velocity;
		}
		if (direction == 2)
		{
			mPosition -= mRight * velocity;
		}
		if (direction == 3)
		{
			mPosition += mRight * velocity;
		}
	}

	void Camera::HandleMouseInput(float xoffset, float yoffset)
	{
		if (!mCanMouse)
		{
			return;
		}
		xoffset *= mMouseSensitivity;
		yoffset *= mMouseSensitivity;

		mYaw += xoffset;
		mPitch += yoffset;

		// Clamp pitch
		if (mPitch > 89.0f)
		{
			mPitch = 89.0f;
		}
		if (mPitch < -89.0f)
		{
			mPitch = -89.0f;
		}

		UpdateCamera();
	}

	void Camera::HandleMouseScroll(float yoffset)
	{
		if (mZoom >= 1.0f && mZoom <= 90.0f)
		{
			mZoom -= yoffset;
		}
		if (mZoom <= 1.0f)
		{
			mZoom = 1.0f;
		}
		if (mZoom > 90.0f)
		{
			mZoom = 90.0f;
		}
	}

	void Camera::HandleMouseButtons(int button, int action)
	{
		// Handle left clicks
		if (button == 0 && action == 1)
		{
			mCanMouse = true;
		}
		if (button == 0 && action == 0)
		{
			mCanMouse = false;
		}
	}

	float Camera::GetZoom()
	{
		return glm::radians(mZoom);
	}

	glm::vec3 Camera::GetPosition()
	{
		return mPosition;
	}

	//void Camera::DisplayDebug()
	//{
	//	if (ImGui::CollapsingHeader("Camera"))
	//	{
	//		// Reset camera to default
	//		if (ImGui::Button("Reset Camera"))
	//		{
	//			Reset();
	//		}
	//
	//		// Display camera information
	//		ImGui::InputFloat3("Position", &mPosition[0]);
	//		ImGui::InputFloat3("Front", &mFront[0]);
	//		ImGui::InputFloat3("Up", &mUp[0]);
	//
	//		ImGui::InputFloat("Pitch", &mPitch);
	//		ImGui::InputFloat("Yaw", &mYaw);
	//		ImGui::InputFloat("Zoom", &mZoom);
	//
	//		ImGui::InputFloat("Movement Speed", &mMovementSpeed);
	//		ImGui::InputFloat("Mouse Sensitivity", &mMouseSensitivity);
	//	}
	//}

	void Camera::UpdateCamera()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
		front.y = sin(glm::radians(mPitch));
		front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
		mFront = glm::normalize(front);

		mRight = glm::normalize(glm::cross(mFront, mWorldUp));
		mUp = glm::normalize(glm::cross(mRight, mFront));
	}

	void Camera::Reset()
	{
		mPosition = glm::vec3(0.0f, 0.0f, 5.0f);
		mUp = glm::vec3(0.0f, 1.0f, 0.0f);
		mFront = glm::vec3(0.0f, 0.0f, -1.0f);
		mWorldUp = mUp;
		mZoom = ZOOM;
		mYaw = YAW;
		mPitch = PITCH;
		UpdateCamera();

	}
}