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
		virtual ~CameraSystem() {}
		void Update();
		void AddGameObject(GameObject* object);

		void HandleKeyboardInput(Camera* pCamera, Transform* pTransform);
		void HandleMouseInput(Camera* pCamera);
		bool HandleMouseScroll(MouseScrolledEvent& mse,Camera* pCamera);

	private:
		void UpdateCamera(Camera* pCamera);
		void Reset(Camera* pCamera);

	private:
		float mLastX;
		float mLastY;
		
	};
}