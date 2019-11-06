#include <hollowpch.h>
#include "CameraSystem.h"
#include "Hollow/Components/Camera.h"

#include"Hollow/Managers/FrameRateController.h"

#include"Hollow/Managers/InputManager.h"
#include"Hollow/Managers/RenderManager.h"
#include "Hollow/Graphics/Data/CameraData.h"
#include "Hollow/Events/MouseEvent.h"
#include "Hollow/Components/Transform.h"


namespace Hollow {
	CameraSystem CameraSystem::instance;
	void CameraSystem::AddGameObject(GameObject* object)
	{
		if (CheckComponents<Transform, Camera>(object))
		{
			Camera* pCamera = object->GetComponent<Camera>();
			UpdateCamera(pCamera);

			if (pCamera->mViewPortSize == glm::ivec2(0))
			{
				glm::vec2 screenSize = RenderManager::Instance().GetWindowSize();
				pCamera->mViewPortSize = screenSize;
				pCamera->mAspectRatio = (screenSize.x / screenSize.y);
			}
			else
			{
				pCamera->mAspectRatio = (pCamera->mViewPortSize.x / pCamera->mViewPortSize.y);
			}
		}
	}
	
	void CameraSystem::HandleKeyboardInput(Camera* pCamera, Transform* pTransform)
	{
		float frameTime = FrameRateController::Instance().GetFrameTime();
		float velocity = pCamera->mMovementSpeed * frameTime;
		if (InputManager::Instance().IsKeyPressed(SDL_SCANCODE_W))
		{
			if (InputManager::Instance().IsMouseButtonPressed(SDL_BUTTON_RIGHT))
			{
				pTransform->mPosition += pCamera->mUp * velocity;
			}
			else
			{
				pTransform->mPosition += pCamera->mFront * velocity;
			}
		}
		if (InputManager::Instance().IsKeyPressed(SDL_SCANCODE_S))
		{
			if (InputManager::Instance().IsMouseButtonPressed(SDL_BUTTON_RIGHT))
			{
				pTransform->mPosition -= pCamera->mUp * velocity;
			}
			else
			{
				pTransform->mPosition -= pCamera->mFront * velocity;
			}
		}
		if (InputManager::Instance().IsKeyPressed(SDL_SCANCODE_A))
		{
			pTransform->mPosition -= pCamera->mRight * velocity;
		}
		if (InputManager::Instance().IsKeyPressed(SDL_SCANCODE_D))
		{
			pTransform->mPosition += pCamera->mRight * velocity;
		}
	}

	void CameraSystem::HandleMouseInput(Camera* pCamera)
	{
		std::pair<float, float> mousePos = InputManager::Instance().GetMousePosition();

		float xoffset = mousePos.first - mLastX;
		float yoffset = mLastY - mousePos.second;

		mLastX = mousePos.first;
		mLastY = mousePos.second;

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

	void CameraSystem::HandleMouseMotion(Camera* pCamera)
	{
		std::pair<float, float> mousePos = InputManager::Instance().GetMouseMotion();

		float xoffset = pCamera->mMouseSensitivity * mousePos.first;
		float yoffset = pCamera->mMouseSensitivity * mousePos.second;
		
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

	void CameraSystem::Update()
	{
		for (unsigned int i = 0; i < mGameObjects.size(); ++i)
		{
			Camera* pCamera = mGameObjects[i]->GetComponent<Camera>();
			Transform* pTransform = mGameObjects[i]->GetComponent<Transform>();

			if (!pCamera->mIsActive)
				continue;
			
			if (pCamera->mType == SCENE_CAMERA)
			{
				HandleKeyboardInput(pCamera, pTransform);
				if (InputManager::Instance().IsMouseButtonPressed(SDL_BUTTON_LEFT) || 
					ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
				{
					HandleMouseInput(pCamera);
				}
			}
			else if(pCamera->mType == MAIN_CAMERA)
			{
				HandleMouseMotion(pCamera);
			}

			CameraData cameraData;
			cameraData.mEyePosition = pTransform->mPosition + glm::toMat3(pTransform->mQuaternion) *  pCamera->mOffsetFromAnchor;

			if (pCamera->mProjectionType == PERSPECTIVE) 
			{
				cameraData.mViewMatrix = glm::lookAt(cameraData.mEyePosition, cameraData.mEyePosition + pCamera->mFront, pCamera->mUp);;
				cameraData.mProjectionMatrix = glm::perspective(pCamera->mZoom, pCamera->mAspectRatio,
					pCamera->mNearPlane, pCamera->mFarPlane);
			}
			else if (pCamera->mProjectionType == ORTHOGRAPHIC)
			{
				cameraData.mViewMatrix = glm::translate(glm::mat4(1.0f), cameraData.mEyePosition);
				cameraData.mProjectionMatrix = glm::ortho(0, pCamera->mViewPortSize.x, 0, pCamera->mViewPortSize.y);
			}
			
			cameraData.mType = pCamera->mType;
			cameraData.mViewPortPosition = pCamera->mViewPortPosition;
			cameraData.mViewPortSize = pCamera->mViewPortSize;
			
			switch (cameraData.mType)
			{
			case MAIN_CAMERA:
				RenderManager::Instance().mMainCamera = cameraData;
				break;
			case UI_CAMERA:
				RenderManager::Instance().mUICamera = cameraData;
				break;
			case SCENE_CAMERA:
				RenderManager::Instance().mMainCamera = cameraData;
				break;
			case SIDE_CAMERA:
				RenderManager::Instance().mSecondaryCameras.push_back(cameraData);
				break;
			}
			
		}

	}

	void CameraSystem::UpdateCamera(Camera* pCamera)
	{		
		glm::vec3 front;
		front.x = cos(glm::radians(pCamera->mYaw)) * cos(glm::radians(pCamera->mPitch));
		front.y = sin(glm::radians(pCamera->mPitch));
		front.z = sin(glm::radians(pCamera->mYaw)) * cos(glm::radians(pCamera->mPitch));
		pCamera->mFront = glm::normalize(front);

		pCamera->mRight = glm::normalize(glm::cross(pCamera->mFront, glm::vec3(0.0f, 1.0f, 0.0f)));
		pCamera->mUp = glm::cross(pCamera->mRight, pCamera->mFront);
	}

	void CameraSystem::Reset(Camera* pCamera)
	{
		pCamera->mUp = glm::vec3(0.0f, 1.0f, 0.0f);
		pCamera->mFront = glm::vec3(0.0f, 0.0f, -1.0f);
		pCamera->mZoom = pCamera->mDefaultZoom;
		pCamera->mYaw = pCamera->mDefaultYaw;
		pCamera->mPitch = pCamera->mDefaultPitch;
		UpdateCamera(pCamera);

	}
}
