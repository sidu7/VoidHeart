#pragma once

#include <glm/glm.hpp>
namespace Hollow {
	class Event;
	class MouseScrolledEvent;

	class Camera
	{
	public:
		Camera(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 3.0f),const glm::vec3& up = glm::vec3(0.0, 1.0, 0.0), float yaw = -90.0f, float pitch = 0.0f);

		glm::mat4 GetViewMatrix();

		void HandleKeyboardInput(float frameTime);
		void HandleMouseInput();
		bool HandleMouseScroll(MouseScrolledEvent& mse);
		void HandleMouseButtons();

		float GetZoom();
		glm::vec3 GetPosition();

		//void DisplayDebug();

		void OnUpdate(float frameTime);
		void OnEvent(Event& e);

	private:
		void UpdateCamera();
		void Reset();

	private:
		glm::vec3 mPosition;
		glm::vec3 mFront;
		glm::vec3 mUp;
		glm::vec3 mRight;
		glm::vec3 mWorldUp;

		float mYaw;
		float mPitch;

		float mMovementSpeed;
		float mMouseSensitivity;
		float mZoom;

		bool mCanMouse;

		static const float YAW;
		static const float PITCH;
		static const float SPEED;
		static const float SENSITIVITY;
		static const float ZOOM;
	};
}