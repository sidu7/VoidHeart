#pragma once

namespace Hollow
{
	class Texture;

	struct MaterialData
	{
		Texture* mpDiffuse;
		Texture* mpSpecular;
		Texture* mpNormal;
		Texture* mpHeight;
	};
}