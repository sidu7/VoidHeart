#pragma once
#include "System.h"

namespace Hollow
{
	class Event;
	class MouseScrolledEvent;
	class Camera;
	class CameraSystem : public System
	{
		REGISTERSYSTEM(CameraSystem, 1)
	public:
		virtual ~CameraSystem() {}
		void Update(GameObject* gameobject);
		void AddGameObject(GameObject* object);

		glm::mat4 GetViewMatrix(Camera* pCamera);

		void HandleKeyboardInput(Camera* pCamera);
		void HandleMouseInput(Camera* pCamera);
		bool HandleMouseScroll(MouseScrolledEvent& mse,Camera* pCamera);
		void HandleMouseButtons(Camera* pCamera);

		float GetZoom(Camera* pCamera);
		glm::vec3 GetPosition(Camera* pCamera);

		//void DisplayDebug();

		//void OnEvent(Event& e);

	private:
		void UpdateCamera(Camera* pCamera);
		void Reset(Camera* pCamera);

	private:
		float mLastX;
		float mLastY;
		
	};
}