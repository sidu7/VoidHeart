#pragma once

namespace Hollow
{
	struct CameraData
	{
		glm::mat4 mViewMatrix;
		glm::vec3 mPosition;
		float mZoom;
		float mNear;
		float mFar;
	};
}