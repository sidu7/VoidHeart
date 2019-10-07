#include <hollowpch.h>
#include "Camera.h"

#include "Hollow/Managers/InputManager.h"
#include "Hollow/Events/MouseEvent.h"

namespace Hollow {

	const float Camera::YAW = -90.0f;
	const float Camera::PITCH = 0.0f;
	const float Camera::SPEED = 5.0f;
	const float Camera::SENSITIVITY = 0.1f;
	const float Camera::ZOOM = 45.0f;

	float lastX = 640;
	float lastY = 360;

	Camera::Camera(const glm::vec3& position,const glm::vec3& up, float yaw, float pitch) : mFront(glm::vec3(0.0f, 0.0f, -1.0f)), mMovementSpeed(SPEED), mMouseSensitivity(SENSITIVITY), mZoom(ZOOM)
	{
		mPosition = position;
		mWorldUp = up;
		mYaw = yaw;
		mPitch = pitch;
		UpdateCamera();
	}

	Camera::~Camera()
	{
	}

	glm::mat4 Camera::GetViewMatrix()
	{
		return glm::lookAt(mPosition, mPosition + mFront, mUp);
	}

	void Camera::HandleKeyboardInput(float frameTime)
	{
		float velocity = mMovementSpeed * frameTime;
		if (InputManager::Instance().IsKeyPressed(SDL_SCANCODE_W))
		{
			if (InputManager::Instance().IsMouseButtonPressed(2))
			{
				mPosition += mUp * velocity;
			}
			else
			{
				mPosition += mFront * velocity;
			}
		}
		if (InputManager::Instance().IsKeyPressed(SDL_SCANCODE_S))
		{
			if (InputManager::Instance().IsMouseButtonPressed(2))
			{
				mPosition -= mUp * velocity;
			}
			else
			{
				mPosition -= mFront * velocity;
			}
		}
		if (InputManager::Instance().IsKeyPressed(SDL_SCANCODE_A))
		{
			mPosition -= mRight * velocity;
		}
		if (InputManager::Instance().IsKeyPressed(SDL_SCANCODE_D))
		{
			mPosition += mRight * velocity;
		}
	}

	void Camera::HandleMouseInput()
	{
		std::pair<float, float> mousePos = InputManager::Instance().GetMousePosition();

		float xoffset = mousePos.first - lastX;
		float yoffset = lastY - mousePos.second;

		lastX = mousePos.first;
		lastY = mousePos.second;

		if (!mCanMouse || ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
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

	bool Camera::HandleMouseScroll(MouseScrolledEvent& mse)
	{
		if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
		{
			if (mZoom >= 1.0f && mZoom <= 90.0f)
			{
				mZoom -= mse.GetYOffset();
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
		return false;
	}

	void Camera::HandleMouseButtons()
	{

		// Handle left clicks
		if (InputManager::Instance().IsMouseButtonPressed(0))
		{
			mCanMouse = true;
		} 
		else
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

	void Camera::OnUpdate(float frameTime)
	{
		HandleKeyboardInput(frameTime);
		HandleMouseButtons();
		HandleMouseInput();
	}

	void Camera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(std::bind(&Camera::HandleMouseScroll, this, std::placeholders::_1));
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