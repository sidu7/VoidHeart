#pragma once
#include "System.h"

namespace Hollow
{
	class Event;
	class MouseScrolledEvent;
	class CameraController;
	class CameraSystem : public System
	{
		REGISTERSYSTEM(CameraSystem, 1)
	public:
		virtual ~CameraSystem() {}
		void Update(GameObject* gameobject);
		void AddGameObject(GameObject* object);

		glm::mat4 GetViewMatrix(CameraController* pCameraController);

		void HandleKeyboardInput(CameraController* pCameraController);
		void HandleMouseInput(CameraController* pCameraController);
		bool HandleMouseScroll(MouseScrolledEvent& mse,CameraController* pCameraController);
		void HandleMouseButtons(CameraController* pCameraController);

		float GetZoom(CameraController* pCameraController);
		glm::vec3 GetPosition(CameraController* pCameraController);

		//void DisplayDebug();

		void OnUpdate(float frameTime);
		void OnEvent(Event& e);

	private:
		void UpdateCamera(CameraController* pCameraController);
		void Reset(CameraController* pCameraController);

	private:
		float mLastX;
		float mLastY;
		
	};
}