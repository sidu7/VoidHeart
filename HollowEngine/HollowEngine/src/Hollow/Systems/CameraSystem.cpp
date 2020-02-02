#include <hollowpch.h>
#include "CameraSystem.h"
#include "Hollow/Core/GameMetaData.h"

#include "Hollow/Components/Camera.h"

#include"Hollow/Managers/FrameRateController.h"

#include"Hollow/Managers/InputManager.h"
#include"Hollow/Managers/RenderManager.h"
#include"Hollow/Managers/GameObjectManager.h"
#include "Hollow/Graphics/Data/CameraData.h"
#include "Hollow/Events/MouseEvent.h"
#include "Hollow/Components/Transform.h"


namespace Hollow {
	CameraSystem CameraSystem::instance;
	void CameraSystem::AddGameObject(GameObject* object)
	{
		if (CheckAllComponents<Transform, Camera>(object))
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
				pCamera->mAspectRatio = ((float)pCamera->mViewPortSize.x / pCamera->mViewPortSize.y);
			}
		}
	}

	void CameraSystem::OnSceneInit()
	{
		for (int i = 0;i< mGameObjects.size(); ++i)
		{
			Camera* pCamera = mGameObjects[i]->GetComponent<Camera>();
			if (pCamera->mType == CameraType::MULTI_FOCUS_CAMERA)
			{
				for (int j = 0; j < pCamera->mFocusObjects.size(); ++j)
				{
					int type = GameMetaData::Instance().mMapOfGameObjectTypes[pCamera->mFocusObjects[j]];
					auto& list = GameObjectManager::Instance().GetObjectByType(type);
					for(auto objects: list)
					{
						pCamera->mFocusPositions.emplace_back(objects->GetComponent<Transform>());
					}
				}
			}
		}
	}
	
	void CameraSystem::HandleKeyboardInput(Camera* pCamera, Transform* pTransform)
	{
		float frameTime = FrameRateController::Instance().GetFrameTime();
		float velocity = pCamera->mMovementSpeed * frameTime;
		if (InputManager::Instance().IsKeyPressed("W")||InputManager::Instance().IsControllerButtonPressed(SDL_CONTROLLER_BUTTON_DPAD_UP))
		{
			if (InputManager::Instance().IsMouseButtonPressed(SDL_BUTTON_RIGHT)|| InputManager::Instance().GetAxisValue(SDL_CONTROLLER_AXIS_TRIGGERRIGHT)>20)
			{
				pTransform->mPosition += pCamera->mUp * velocity;
			}
			else
			{
				pTransform->mPosition += pCamera->mFront * velocity;
			}
		}
		if (InputManager::Instance().IsKeyPressed("S")||InputManager::Instance().IsControllerButtonPressed(SDL_CONTROLLER_BUTTON_DPAD_DOWN))
		{
			if (InputManager::Instance().IsMouseButtonPressed(SDL_BUTTON_RIGHT) || InputManager::Instance().GetAxisValue(SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > 20)
			{
				pTransform->mPosition -= pCamera->mUp * velocity;
			}
			else
			{
				pTransform->mPosition -= pCamera->mFront * velocity;
			}
		}
		if (InputManager::Instance().IsKeyPressed("A")||InputManager::Instance().IsControllerButtonPressed(SDL_CONTROLLER_BUTTON_DPAD_LEFT))
		{
			pTransform->mPosition -= pCamera->mRight * velocity;
		}
		if (InputManager::Instance().IsKeyPressed("D") || InputManager::Instance().IsControllerButtonPressed(SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
		{
			pTransform->mPosition += pCamera->mRight * velocity;
		}
	}

	void CameraSystem::HandleMouseInput(Camera* pCamera)
	{
		glm::vec2 mousePos = InputManager::Instance().GetMousePosition();

		float xoffset = mousePos.x - mLastX;
		float yoffset = mLastY - mousePos.y;

		mLastX = mousePos.x;
		mLastY = mousePos.y;

		if (!InputManager::Instance().IsMouseButtonPressed(SDL_BUTTON_RIGHT)&&abs(InputManager::Instance().GetAxisValue(SDL_CONTROLLER_AXIS_TRIGGERLEFT))<20)// ||
			//ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
		{
			return;
		}
		if (abs(InputManager::Instance().GetAxisValue(SDL_CONTROLLER_AXIS_RIGHTX)) > 8000|| abs(InputManager::Instance().GetAxisValue(SDL_CONTROLLER_AXIS_RIGHTY)) > 8000)
		{
			xoffset = (float)InputManager::Instance().GetAxisValue(SDL_CONTROLLER_AXIS_RIGHTX) / 16000;
			yoffset = (float)InputManager::Instance().GetAxisValue(SDL_CONTROLLER_AXIS_RIGHTY) / -16000;
		}
		else if (InputManager::Instance().IsMouseButtonPressed(SDL_BUTTON_RIGHT))
		{
			xoffset *= pCamera->mMouseSensitivity;
			yoffset *= pCamera->mMouseSensitivity;
		}
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
		glm::vec2 mousePos = InputManager::Instance().GetMouseMotion();
		float xoffset, yoffset;
		
		if (abs(InputManager::Instance().GetAxisValue(SDL_CONTROLLER_AXIS_RIGHTX)) > 8000 || abs(InputManager::Instance().GetAxisValue(SDL_CONTROLLER_AXIS_RIGHTY)) > 8000)
		{
			xoffset = (float)InputManager::Instance().GetAxisValue(SDL_CONTROLLER_AXIS_RIGHTX)/16000;
			yoffset = (float)InputManager::Instance().GetAxisValue(SDL_CONTROLLER_AXIS_RIGHTY)/-16000;
		}
		else
		{
			xoffset = pCamera->mMouseSensitivity * mousePos.x;
			yoffset = pCamera->mMouseSensitivity * mousePos.y;
		}
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
				HandleMouseInput(pCamera);
			}
			else if(pCamera->mType == MAIN_CAMERA||pCamera->mType ==THIRD_PERSON_CAMERA)
			{
				HandleMouseMotion(pCamera);
			}

			CameraData cameraData;

			if (pCamera->mType == TOP_DOWN_CAMERA)
			{
				UpdateTopDownCamera(pCamera, pTransform, cameraData);
			}
			else if (pCamera->mType == MULTI_FOCUS_CAMERA)
			{
				UpdateMultiFocusCamera(pCamera,pTransform,cameraData);
			}
			else if (pCamera->mType != THIRD_PERSON_CAMERA)
			{
				cameraData.mEyePosition = pTransform->mPosition + glm::toMat3(pTransform->mQuaternion) * pCamera->mOffsetFromAnchor;

				if (pCamera->mProjectionType == PERSPECTIVE)
				{
					cameraData.mViewMatrix = glm::lookAt(cameraData.mEyePosition, cameraData.mEyePosition + pCamera->mFront, pCamera->mUp);;
					cameraData.mProjectionMatrix = glm::perspective(pCamera->mZoom, pCamera->mAspectRatio,
						pCamera->mNearPlane, pCamera->mFarPlane);
				}
				else if (pCamera->mProjectionType == ORTHOGRAPHIC)
				{
					cameraData.mProjectionMatrix = glm::ortho(0.0f, static_cast<float>(pCamera->mViewPortSize.x), 0.0f, static_cast<float>(pCamera->mViewPortSize.y), -1.0f, 1.0f);
					cameraData.mViewMatrix = glm::translate(glm::mat4(1.0f), cameraData.mEyePosition);
				}
			}
			else
			{
				glm::vec3 frontDirection = - pCamera->mOffsetFromAnchor;
				// Clamp pitch
				if (pCamera->mPitch > pCamera->mPitchLowerBound)
				{
					pCamera->mPitch = pCamera->mPitchLowerBound;
				}
				frontDirection = glm::rotateX(frontDirection, glm::radians(-pCamera->mPitch));
				frontDirection = glm::rotateY(frontDirection, glm::radians(-pCamera->mYaw));

				cameraData.mEyePosition = frontDirection + pTransform->mPosition;
				pCamera->mFront = glm::normalize(pTransform->mPosition + pCamera->mAnchorFocusOffset - cameraData.mEyePosition);
				pCamera->mRight = glm::normalize(glm::cross(pCamera->mFront, glm::vec3(0.0f, 1.0f, 0.0f)));
				pCamera->mUp = glm::normalize(glm::cross(pCamera->mRight, pCamera->mFront));
				cameraData.mViewMatrix = glm::lookAt(cameraData.mEyePosition, cameraData.mEyePosition + pCamera->mFront, pCamera->mUp);;
				cameraData.mProjectionMatrix = glm::perspective(pCamera->mZoom, pCamera->mAspectRatio,
					pCamera->mNearPlane, pCamera->mFarPlane);
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
			case TOP_DOWN_CAMERA:
				RenderManager::Instance().mMainCamera = cameraData;
				break;
			case THIRD_PERSON_CAMERA:
				RenderManager::Instance().mMainCamera = cameraData;
				break;
			case MULTI_FOCUS_CAMERA:
				RenderManager::Instance().mMainCamera = cameraData;
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

	void CameraSystem::UpdateTopDownCamera(Camera* pCamera, Transform* pTransform, CameraData& cameraData)
	{
		// Set camera position based on object
		// LERP to desired position
		glm::vec3 desiredPosition = pTransform->mPosition + pCamera->mOffsetFromAnchor;
		glm::vec3& p = desiredPosition;
		float t = pCamera->mLERPFactor;
		glm::vec3 d = pCamera->mPreviousPosition + t * (desiredPosition - pCamera->mPreviousPosition);
		cameraData.mEyePosition = d;
		// Apply camera constraits if they exist
		ApplyConstraints(pCamera, cameraData);
		pCamera->mPreviousPosition = cameraData.mEyePosition;

		glm::vec3 frontDirection = glm::normalize(glm::vec3(0.0f, -0.9f, -1.0f));
		pCamera->mFront = frontDirection;
		pCamera->mRight = glm::normalize(glm::cross(pCamera->mFront, glm::vec3(0.0f, 1.0f, 0.0f)));
		pCamera->mUp = glm::normalize(glm::cross(pCamera->mRight, pCamera->mFront));

		// Set cameradata matrices
		cameraData.mViewMatrix = glm::lookAt(cameraData.mEyePosition, cameraData.mEyePosition + pCamera->mFront, pCamera->mUp);;
		cameraData.mProjectionMatrix = glm::perspective(pCamera->mZoom, pCamera->mAspectRatio,
			pCamera->mNearPlane, pCamera->mFarPlane);
	}

	void CameraSystem::UpdateMultiFocusCamera(Camera* pCamera, Transform* pTransform, CameraData& cameraData)
	{
		// Set camera position based on object
		// LERP to desired position
		float distance = 0.0f;
		glm::vec3 midPoint(0.0f);
		for (int i = 0;i<pCamera->mFocusPositions.size();++i)
		{
			for (int j = 0; j < pCamera->mFocusPositions.size(); ++j)
			{
				if (i == j)
					continue;
				glm::vec3 objectsvector = pCamera->mFocusPositions[i]->mPosition - pCamera->mFocusPositions[j]->mPosition;
				float tempdist = glm::length(objectsvector);
				if (tempdist >= distance)
					distance = tempdist;
			}
			midPoint += pCamera->mFocusPositions[i]->mPosition;
		}
		midPoint /= pCamera->mFocusPositions.size();
		glm::vec3 focuspoint = midPoint;
		glm::vec3 desiredPosition = focuspoint + pCamera->mOffsetFromAnchor;
		glm::vec3 apnaFront = glm::normalize(pCamera->mOffsetFromAnchor);
		desiredPosition += apnaFront * distance * 0.5f;
		glm::vec3& p = desiredPosition;
		float t = pCamera->mLERPFactor;
		glm::vec3 d = pCamera->mPreviousPosition + t * (desiredPosition - pCamera->mPreviousPosition);
		cameraData.mEyePosition = d;
		// Apply camera constraits if they exist
		//ApplyConstraints(pCamera, cameraData);
		pCamera->mPreviousPosition = cameraData.mEyePosition;

		glm::vec3 frontDirection = glm::normalize(glm::vec3(0.0f, -0.5f, -1.0f));
		//glm::vec3 frontDirection = glm::normalize(focuspoint - desiredPosition);
		pCamera->mFront = frontDirection;
		pCamera->mRight = glm::normalize(glm::cross(pCamera->mFront, glm::vec3(0.0f, 1.0f, 0.0f)));
		pCamera->mUp = glm::normalize(glm::cross(pCamera->mRight, pCamera->mFront));

		// Set cameradata matrices
		cameraData.mViewMatrix = glm::lookAt(cameraData.mEyePosition, cameraData.mEyePosition + pCamera->mFront, pCamera->mUp);;
		cameraData.mProjectionMatrix = glm::perspective(pCamera->mZoom, pCamera->mAspectRatio,
			pCamera->mNearPlane, pCamera->mFarPlane);
	}
	void CameraSystem::ApplyConstraints(Camera* pCamera, CameraData& cameraData)
	{
		glm::vec2& x = pCamera->mXConstraints;
		glm::vec2& y = pCamera->mYConstraints;
		glm::vec2& z = pCamera->mZConstraints;

		cameraData.mEyePosition.x = glm::max(x.x, glm::min(cameraData.mEyePosition.x, x.y));
		cameraData.mEyePosition.y = glm::max(y.x, glm::min(cameraData.mEyePosition.y, y.y));
		cameraData.mEyePosition.z = glm::max(z.x, glm::min(cameraData.mEyePosition.z, z.y));
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
