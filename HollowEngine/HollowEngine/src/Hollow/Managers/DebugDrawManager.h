#pragma once
#include "Hollow/Common.h"
#include "Hollow/Graphics/Data/Color.h"

namespace Hollow
{
	class DebugDrawManager
	{
		SINGLETON(DebugDrawManager)
	public:
		HOLLOW_API void Init();
		HOLLOW_API void CleanUp();

		HOLLOW_API void DebugDirectionLine(glm::vec3 startPos, glm::vec3 endPos, glm::vec3 color = COLOR_GREEN);
		HOLLOW_API void DebugRay(glm::vec3 startPos, glm::vec3 direction, glm::vec3 color = COLOR_RED);
		HOLLOW_API void DebugLine(glm::vec3 startPos, glm::vec3 endPos, glm::vec3 color = COLOR_GREEN);
		HOLLOW_API void DebugCircle(glm::vec3 position, glm::vec2 scale, glm::vec3 color = COLOR_GREEN);
		HOLLOW_API void DebugQuad(glm::vec3 position = glm::vec3(1.0f), glm::vec2 scale = glm::vec2(1.0f), glm::vec3 color = COLOR_GREEN);
		HOLLOW_API void DebugAxes(glm::vec3 position, glm::vec3 scale);
		HOLLOW_API void DebugSphere(glm::vec3 position = glm::vec3(1.0f), glm::vec3 scale = glm::vec3(1.0f), glm::vec3 color = COLOR_GREEN);
		HOLLOW_API void DebugCube(glm::vec3 position = glm::vec3(1.0f), glm::vec3 scale = glm::vec3(1.0f), glm::vec3 color = COLOR_GREEN);
		HOLLOW_API void DebugCube(glm::mat4& modelMatrix, glm::vec3 color = COLOR_GREEN);
	};
}