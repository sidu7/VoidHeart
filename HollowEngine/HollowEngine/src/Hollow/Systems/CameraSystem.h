#pragma once
#include "System.h"

namespace Hollow
{
	class Transform;
	class Event;
	class MouseScrolledEvent;
	class Camera;
	struct CameraData;

	/// <summary>
	/// Class CameraSystem.
	/// Inherits from the <see cref="System" /> class.
	/// </summary>
	/// <seealso cref="System" />
	class CameraSystem : public System
	{
		/// <summary>
		/// Registers this System with the SystemManager
		/// </summary>
		REGISTERSYSTEM(CameraSystem, 1)
	public:
		HOLLOW_API virtual ~CameraSystem() {}
		/// <summary>
		/// Updates the System every frame.
		/// </summary>
		HOLLOW_API void Update();
		/// <summary>
		/// Adds the game object.
		/// </summary>
		/// <param name="object">The object to add.</param>
		HOLLOW_API void AddGameObject(GameObject* object);
		/// <summary>
		/// Called when a scene is initialized.
		/// </summary>
		HOLLOW_API void OnSceneInit() override;

	private:
		void HandleKeyboardInput(Camera* pCamera, Transform* pTransform);
		void HandleMouseInput(Camera* pCamera);
		void HandleMouseMotion(Camera* pCamera);
		bool HandleMouseScroll(MouseScrolledEvent& mse,Camera* pCamera);
		void UpdateCamera(Camera* pCamera);
		void UpdateTopDownCamera(Camera* pCamera, Transform* pTransform, CameraData& cameraData);
		void UpdateCenterOnRoomCamera(Camera* pCamera, Transform* pTransform, CameraData& cameraData, GameObject* pGO);
		void UpdateMultiFocusCamera(Camera* pCamera, Transform* pTransform, CameraData& cameraData, GameObject* pGO);
		void ApplyConstraints(Camera* pCamera, CameraData& cameraData);
		void Reset(Camera* pCamera);

	private:
		float mLastX;
		float mLastY;
		
	};
}