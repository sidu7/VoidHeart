#pragma once

namespace Hollow
{
	class Texture;
	class Mesh;

	struct UIRenderData
	{
		glm::mat4 mModelTransform;
		Texture* mpTexture;
		Mesh* mpShape;
	};
}