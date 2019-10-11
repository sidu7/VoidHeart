#include <hollowpch.h>
#include "CameraSystem.h"
#include "Hollow/Components/CameraController.h"
#include"Hollow/Managers/FrameRateController.h"
#include"Hollow/Managers/InputManager.h"
#include"Hollow/Managers/RenderManager.h"
#include "Hollow/Graphics/RenderData.h"
#include "Hollow/Events/MouseEvent.h"

namespace Hollow {
	void CameraSystem::AddGameObject(GameObject* object)
	{
		CheckComponents<CameraController>(object);
	}
	void Update(GameObject* gameobject)
	{

	}

	glm::mat4 CameraSystem::GetViewMatrix(CameraController* pCameraController)
	{
		return glm::lookAt(pCameraController->mPosition, pCameraController->mPosition + pCameraController->mFront, pCameraController->mUp);
	}

	void CameraSystem::HandleKeyboardInput(CameraController* pCameraController)
	{
		float frameTime = FrameRateController::Instance().GetFrameTime();
		float velocity = pCameraController->mMovementSpeed * frameTime;
		if (InputManager::Instance().IsKeyPressed(SDL_SCANCODE_W))
		{
			if (InputManager::Instance().IsMouseButtonPressed(2))
			{
				pCameraController->mPosition += pCameraController->mUp * velocity;
			}
			else
			{
				pCameraController->mPosition += pCameraController->mFront * velocity;
			}
		}
		if (InputManager::Instance().IsKeyPressed(SDL_SCANCODE_S))
		{
			if (InputManager::Instance().IsMouseButtonPressed(2))
			{
				pCameraController->mPosition -= pCameraController->mUp * velocity;
			}
			else
			{
				pCameraController->mPosition -= pCameraController->mFront * velocity;
			}
		}
		if (InputManager::Instance().IsKeyPressed(SDL_SCANCODE_A))
		{
			pCameraController->mPosition -= pCameraController->mRight * velocity;
		}
		if (InputManager::Instance().IsKeyPressed(SDL_SCANCODE_D))
		{
			pCameraController->mPosition += pCameraController->mRight * velocity;
		}
	}

	void CameraSystem::HandleMouseInput(CameraController* pCameraController)
	{
		std::pair<float, float> mousePos = InputManager::Instance().GetMousePosition();

		float xoffset = mousePos.first - mLastX;
		float yoffset = mLastY - mousePos.second;

		mLastX = mousePos.first;
		mLastY = mousePos.second;

		if (!pCameraController->mCanMouse || ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
		{
			return;
		}

		xoffset *= pCameraController->mMouseSensitivity;
		yoffset *= pCameraController->mMouseSensitivity;

		pCameraController->mYaw += xoffset;
		pCameraController->mPitch += yoffset;

		// Clamp pitch
		if (pCameraController->mPitch > 89.0f)
		{
			pCameraController->mPitch = 89.0f;
		}
		if (pCameraController->mPitch < -89.0f)
		{
			pCameraController->mPitch = -89.0f;
		}

		UpdateCamera(pCameraController);
	}

	bool CameraSystem::HandleMouseScroll(MouseScrolledEvent& mse,CameraController* pCameraController)
	{
		if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
		{
			if (pCameraController->mZoom >= 1.0f && pCameraController->mZoom <= 90.0f)
			{
				pCameraController->mZoom -= mse.GetYOffset();
			}
			if (pCameraController->mZoom <= 1.0f)
			{
				pCameraController->mZoom = 1.0f;
			}
			if (pCameraController->mZoom > 90.0f)
			{
				pCameraController->mZoom = 90.0f;
			}
		}
		return false;
	}

	void CameraSystem::HandleMouseButtons(CameraController* pCameraController)
	{

		// Handle left clicks
		if (InputManager::Instance().IsMouseButtonPressed(0))
		{
			pCameraController->mCanMouse = true;
		}
		else
		{
			pCameraController->mCanMouse = false;
		}
	}

	float CameraSystem::GetZoom(CameraController* pCameraController)
	{
		return glm::radians(pCameraController->mZoom);
	}

	glm::vec3 CameraSystem::GetPosition(CameraController* pCameraController)
	{
		return pCameraController->mPosition;
	}

	void CameraSystem::Update(GameObject* gameobject)
	{
		CameraController* pCameraController = gameobject->GetComponent<CameraController>();

		HandleKeyboardInput(pCameraController);
		HandleMouseButtons(pCameraController);
		HandleMouseInput(pCameraController);

		CameraData cameraData;
		cameraData.mZoom = pCameraController->mZoom;
		cameraData.mNear = pCameraController->mNear;
		cameraData.mFar = pCameraController->mFar;
		cameraData.mViewMatrix = GetViewMatrix(pCameraController);

		RenderManager::Instance().mCameraData.push_back(cameraData);
		
	}

	void CameraSystem::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(std::bind(&HandleMouseScroll, this, std::placeholders::_1));
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

	void CameraSystem::UpdateCamera(CameraController* pCameraController)
	{
		glm::vec3 front;
		front.x = cos(glm::radians(pCameraController->mYaw)) * cos(glm::radians(pCameraController->mPitch));
		front.y = sin(glm::radians(pCameraController->mPitch));
		front.z = sin(glm::radians(pCameraController->mYaw)) * cos(glm::radians(pCameraController->mPitch));
		pCameraController->mFront = glm::normalize(front);

		pCameraController->mRight = glm::normalize(glm::cross(pCameraController->mFront, pCameraController->mWorldUp));
		pCameraController->mUp = glm::cross(pCameraController->mRight, pCameraController->mFront);
	}

	void CameraSystem::Reset(CameraController* pCameraController)
	{
		pCameraController->mPosition = glm::vec3(0.0f, 0.0f, 5.0f);
		pCameraController->mUp = glm::vec3(0.0f, 1.0f, 0.0f);
		pCameraController->mFront = glm::vec3(0.0f, 0.0f, -1.0f);
		pCameraController->mWorldUp = pCameraController->mUp;
		pCameraController->mZoom = pCameraController->ZOOM;
		pCameraController->mYaw = pCameraController->YAW;
		pCameraController->mPitch = pCameraController->PITCH;
		UpdateCamera(pCameraController);

	}
}