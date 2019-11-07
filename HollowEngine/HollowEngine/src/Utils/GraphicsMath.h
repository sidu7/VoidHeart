#pragma once
#include "Hollow/Core/Core.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace Hollow
{
	class HOLLOW_API GraphicsMath
	{
	public:
		inline static glm::mat4 RotationFromDirection(const glm::vec3& direction)
		{
			float angle = std::atan2(direction.y, direction.x);
			glm::mat4 glmrotXY = glm::rotate(angle, glm::vec3(0.0f, 0.0f, 1.0f));
			// Find the angle with the xy with plane (0, 0, 1); the - there is because we want to 
			// 'compensate' for that angle (a 'counter-angle')
			float angleZ = -std::asin(direction.z);
			// Make the matrix for that, assuming that Y is your 'side' vector; makes the model 'pitch'
			glm::mat4 glmrotZ = glm::rotate(angleZ, glm::vec3(0.0f, 1.0f, 0.0f));
			return glmrotXY * glmrotZ;
		}

		inline static glm::vec3 TranslateFromMatrix(const glm::mat4& matrix)
		{
			return xyz(matrix[3]);
		}

		inline static glm::vec3 ScaleFromMatrix(const glm::mat4& matrix)
		{
			glm::vec3 a = xyz(matrix[0]);
			glm::vec3 b = xyz(matrix[1]);
			glm::vec3 c = xyz(matrix[2]);

			return glm::vec3(glm::length(a), glm::length(b), glm::length(c));
		}

		inline static glm::quat RotationFromMatrix(glm::mat4 matrix)
		{
			matrix[3].x = 0.0f; matrix[3].y = 0.0f; matrix[3].z = 0.0f;

			glm::vec3 a = xyz(matrix[0]);
			glm::vec3 b = xyz(matrix[1]);
			glm::vec3 c = xyz(matrix[2]);

			glm::vec3 s = glm::vec3(glm::length(a), glm::length(b), glm::length(c));

			matrix[0] /= s.x;
			matrix[1] /= s.y;
			matrix[2] /= s.z;

			return glm::toQuat(matrix);
		}

		inline static std::pair<glm::vec3, glm::quat> ScaleRotationFromMatrix(glm::mat4 matrix)
		{
			matrix[3].x = 0.0f; matrix[3].y = 0.0f; matrix[3].z = 0.0f;

			glm::vec3 a = xyz(matrix[0]);
			glm::vec3 b = xyz(matrix[1]);
			glm::vec3 c = xyz(matrix[2]);

			glm::vec3 s = glm::vec3(glm::length(a), glm::length(b), glm::length(c));

			matrix[0] /= s.x;
			matrix[1] /= s.y;
			matrix[2] /= s.z;

			return std::make_pair(s, glm::toQuat(matrix));
		}
	};
}