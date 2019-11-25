#pragma once

namespace Hollow
{
	enum CameraType
	{
		MAIN_CAMERA,
		UI_CAMERA,
		SIDE_CAMERA,
		SCENE_CAMERA,
		THIRD_PERSON_CAMERA,
		TOP_DOWN_CAMERA
	};

	enum CameraProjection
	{
		PERSPECTIVE,
		ORTHOGRAPHIC
	};
	
	struct CameraData
	{
		CameraType mType;
		glm::mat4 mViewMatrix;
		glm::mat4 mProjectionMatrix;
		glm::ivec2 mViewPortPosition;
		glm::ivec2 mViewPortSize;
		glm::vec3 mEyePosition;
	};
}