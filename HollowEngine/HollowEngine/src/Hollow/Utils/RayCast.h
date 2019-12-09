#pragma once
#include <glm/glm.hpp>

/// <summary>
/// Struct Ray
/// </summary>
struct Ray {
	glm::vec3 origin;
	glm::vec3 direction;
};

/// <summary>
/// Struct IntersectionData
/// </summary>
struct IntersectionData {
	float depth;
	void* object;
};