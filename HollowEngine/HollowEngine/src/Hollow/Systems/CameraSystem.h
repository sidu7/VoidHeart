#pragma once
#include "System.h"

namespace Hollow
{
	class Transform;
	class Event;
	class MouseScrolledEvent;
	class Camera;
	class CameraSystem : public System
	{
		REGISTERSYSTEM(CameraSystem, 1)
	public:
		HOLLOW_API virtual ~CameraSystem() {}
		HOLLOW_API void Update();
		HOLLOW_API void AddGameObject(GameObject* object);		 

	private:
		void HandleKeyboardInput(Camera* pCamera, Transform* pTransform);
		void HandleMouseInput(Camera* pCamera);
		void HandleMouseMotion(Camera* pCamera);
		bool HandleMouseScroll(MouseScrolledEvent& mse,Camera* pCamera);
		void UpdateCamera(Camera* pCamera);
		void Reset(Camera* pCamera);

	private:
		float mLastX;
		float mLastY;
		
	};
}