#include <hollowpch.h>
#include "CameraSystem.h"
#include "Hollow/Components/Camera.h"

#include"Hollow/Managers/FrameRateController.h"

#include"Hollow/Managers/InputManager.h"
#include"Hollow/Managers/RenderManager.h"
#include "Hollow/Graphics/Data/CameraData.h"
#include "Hollow/Events/MouseEvent.h"


namespace Hollow {
	CameraSystem CameraSystem::instance;
	void CameraSystem::AddGameObject(GameObject* object)
	{
		if (CheckComponents<Camera>(object))
			UpdateCamera(object->GetComponent<Camera>());
	}
	void Update(GameObject* gameobject)
	{

	}

	glm::mat4 CameraSystem::GetViewMatrix(Camera* pCamera)
	{
		return glm::lookAt(pCamera->mPosition, pCamera->mPosition + pCamera->mFront, pCamera->mUp);
	}

	void CameraSystem::HandleKeyboardInput(Camera* pCamera)
	{
		float frameTime = FrameRateController::Instance().GetFrameTime();
		float velocity = pCamera->mMovementSpeed * frameTime;
		if (InputManager::Instance().IsKeyPressed(SDL_SCANCODE_W))
		{
			if (InputManager::Instance().IsMouseButtonPressed(2))
			{
				pCamera->mPosition += pCamera->mUp * velocity;
			}
			else
			{
				pCamera->mPosition += pCamera->mFront * velocity;
			}
		}
		if (InputManager::Instance().IsKeyPressed(SDL_SCANCODE_S))
		{
			if (InputManager::Instance().IsMouseButtonPressed(2))
			{
				pCamera->mPosition -= pCamera->mUp * velocity;
			}
			else
			{
				pCamera->mPosition -= pCamera->mFront * velocity;
			}
		}
		if (InputManager::Instance().IsKeyPressed(SDL_SCANCODE_A))
		{
			pCamera->mPosition -= pCamera->mRight * velocity;
		}
		if (InputManager::Instance().IsKeyPressed(SDL_SCANCODE_D))
		{
			pCamera->mPosition += pCamera->mRight * velocity;
		}
	}

	void CameraSystem::HandleMouseInput(Camera* pCamera)
	{
		std::pair<float, float> mousePos = InputManager::Instance().GetMousePosition();

		float xoffset = mousePos.first - mLastX;
		float yoffset = mLastY - mousePos.second;

		mLastX = mousePos.first;
		mLastY = mousePos.second;

		if (!pCamera->mCanMouse || ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
		{
			return;
		}

		xoffset *= pCamera->mMouseSensitivity;
		yoffset *= pCamera->mMouseSensitivity;

		pCamera->mYaw += xoffset;
		pCamera->mPitch += yoffset;

		// Clamp pitch
		if (pCamera->mPitch > 89.0f)
		{
			pCamera->mPitch = 89.0f;
		}
		if (pCamera->mPitch < -89.0f)
		{
			pCamera->mPitch = -89.0f;
		}

		UpdateCamera(pCamera);
	}

	bool CameraSystem::HandleMouseScroll(MouseScrolledEvent& mse, Camera* pCamera)
	{
		if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
		{
			if (pCamera->mZoom >= 1.0f && pCamera->mZoom <= 90.0f)
			{
				pCamera->mZoom -= mse.GetYOffset();
			}
			if (pCamera->mZoom <= 1.0f)
			{
				pCamera->mZoom = 1.0f;
			}
			if (pCamera->mZoom > 90.0f)
			{
				pCamera->mZoom = 90.0f;
			}
		}
		return false;
	}

	void CameraSystem::HandleMouseButtons(Camera* pCamera)
	{

		// Handle left clicks
		if (InputManager::Instance().IsMouseButtonPressed(0))
		{
			pCamera->mCanMouse = true;
		}
		else
		{
			pCamera->mCanMouse = false;
		}
	}

	float CameraSystem::GetZoom(Camera* pCamera)
	{
		return glm::radians(pCamera->mZoom);
	}

	glm::vec3 CameraSystem::GetPosition(Camera* pCamera)
	{
		return pCamera->mPosition;
	}

	void CameraSystem::Update()
	{
		for (unsigned int i = 0; i < mGameObjects.size(); ++i)
		{
			Camera* pCamera = mGameObjects[i]->GetComponent<Camera>();

			HandleKeyboardInput(pCamera);
			HandleMouseButtons(pCamera);
			HandleMouseInput(pCamera);

			CameraData cameraData;
			cameraData.mPosition = pCamera->mPosition;
			cameraData.mZoom = GetZoom(pCamera);
			cameraData.mNearPlane = pCamera->mNearPlane;
			cameraData.mFarPlane = pCamera->mFarPlane;
			cameraData.mViewMatrix = GetViewMatrix(pCamera);
			cameraData.mProjection = pCamera->mProjection;
			cameraData.mType = pCamera->mType;
			cameraData.mScreenPosition = pCamera->mScreenPosition;
			cameraData.mScreenViewPort = pCamera->mViewPort;

			RenderManager::Instance().mCameraData.push_back(cameraData);
		}

	}

	/*void CameraSystem::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(std::bind(&CameraSystem::HandleMouseScroll, this, std::placeholders::_1));
	}*/

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

	void CameraSystem::UpdateCamera(Camera* pCamera)
	{
		glm::vec3 front;
		front.x = cos(glm::radians(pCamera->mYaw)) * cos(glm::radians(pCamera->mPitch));
		front.y = sin(glm::radians(pCamera->mPitch));
		front.z = sin(glm::radians(pCamera->mYaw)) * cos(glm::radians(pCamera->mPitch));
		pCamera->mFront = glm::normalize(front);

		pCamera->mRight = glm::normalize(glm::cross(pCamera->mFront, pCamera->mWorldUp));
		pCamera->mUp = glm::cross(pCamera->mRight, pCamera->mFront);
	}

	void CameraSystem::Reset(Camera* pCamera)
	{
		pCamera->mPosition = glm::vec3(0.0f, 0.0f, 5.0f);
		pCamera->mUp = glm::vec3(0.0f, 1.0f, 0.0f);
		pCamera->mFront = glm::vec3(0.0f, 0.0f, -1.0f);
		pCamera->mWorldUp = pCamera->mUp;
		pCamera->mZoom = pCamera->mDefaultZoom;
		pCamera->mYaw = pCamera->mDefaultYaw;
		pCamera->mPitch = pCamera->mDefaultPitch;
		UpdateCamera(pCamera);

	}
}