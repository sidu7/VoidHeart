#pragma once

namespace Hollow
{
	enum CameraType
	{
		MAIN_CAMERA,
		UI_CAMERA,
		SIDE_CAMERA,
		SCENE_CAMERA
	};

	enum CameraProjection
	{
		PERSPECTIVE,
		ORTHOGRAPHIC
	};

	struct CameraData
	{
		glm::mat4 mViewMatrix;
		glm::vec3 mPosition;
		float mZoom;
		float mNearPlane;
		float mFarPlane;
		glm::ivec2 mScreenPosition;
		glm::ivec2 mScreenViewPort;
		CameraProjection mProjection;
		CameraType mType;
	};
}