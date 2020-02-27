#pragma once

namespace Hollow
{
	class Texture;
	class Mesh;
	struct UITextData;

	struct UIRenderData
	{
		glm::mat4 mModelTransform;
		Texture* mpTexture;
		UITextData* mpTextData;
		Mesh* mpShape;
		glm::vec3 mColor;
		float mAlpha;
		int mLayer;
	};
}