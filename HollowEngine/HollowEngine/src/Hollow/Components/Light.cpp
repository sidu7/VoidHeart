#include <hollowpch.h>
#include "Light.h"

#include "Hollow/Graphics/FrameBuffer.h"

namespace Hollow
{
	Light Light::instance;

	void Light::Init()
	{
	}

	void Light::Serialize(rapidjson::Value::Object data)
	{
		if (data.HasMember("Color"))
		{
			mColor = JSONHelper::GetVec3F(data["Color"].GetArray());
		}
		if (data.HasMember("CastShadow"))
		{
			mCastShadow = data["CastShadow"].GetBool();
		}
		if (data.HasMember("ShadowMapSize"))
		{
			glm::vec2 size = JSONHelper::GetVec2F(data["ShadowMapSize"].GetArray());
			mpShadowMap = new FrameBuffer(size.x, size.y, 1, true);
		}
	}

	void Light::Clear()
	{
	}

	void Light::DebugDisplay()
	{
	}
}