#pragma once

#include <glm/glm.hpp>

namespace Hollow
{
#ifndef VERTEX
#define VERTEX
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 tex;
		glm::vec3 tangent;
	};
#endif
}