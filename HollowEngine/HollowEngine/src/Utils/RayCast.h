#pragma once
#include <glm/glm.hpp>

class GameObject;

struct Ray {
	glm::vec3 origin;
	glm::vec3 direction;
};

struct IntersectionData {
	float depth;
	void* object;
};

class RayCast
{
	static void* castRay(float x, float y)
	{
		
	}
};