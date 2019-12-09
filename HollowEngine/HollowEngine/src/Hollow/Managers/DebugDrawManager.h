#pragma once
#include "Hollow/Common.h"
#include "Hollow/Graphics/Data/Color.h"

namespace Hollow
{
	/// <summary>
	/// Class Debug Drawing Manager.
	/// </summary>
	class DebugDrawManager
	{
		SINGLETON(DebugDrawManager)
	public:
		
		/// <summary>
		/// Initializes manager data.
		/// </summary>
		HOLLOW_API void Init();
		
		/// <summary>
		/// Cleans manager data.
		/// </summary>
		HOLLOW_API void CleanUp();

		/// <summary>
		/// Draws debug directional line.
		/// </summary>
		/// <param name="startPos">The start position.</param>
		/// <param name="endPos">The end position.</param>
		/// <param name="color">The color.</param>
		HOLLOW_API void DebugDirectionLine(glm::vec3 startPos, glm::vec3 endPos, glm::vec3 color = COLOR_GREEN);
		
		/// <summary>
		/// Draws debug ray.
		/// </summary>
		/// <param name="startPos">The start position.</param>
		/// <param name="direction">The direction vector.</param>
		/// <param name="color">The color.</param>
		HOLLOW_API void DebugRay(glm::vec3 startPos, glm::vec3 direction, glm::vec3 color = COLOR_RED);
		
		/// <summary>
		/// Draws debug line.
		/// </summary>
		/// <param name="startPos">The start position.</param>
		/// <param name="endPos">The end position.</param>
		/// <param name="color">The color.</param>
		HOLLOW_API void DebugLine(glm::vec3 startPos, glm::vec3 endPos, glm::vec3 color = COLOR_GREEN);
		
		/// <summary>
		/// Draws debug circle.
		/// </summary>
		/// <param name="position">The position.</param>
		/// <param name="scale">The scale.</param>
		/// <param name="color">The color.</param>
		HOLLOW_API void DebugCircle(glm::vec3 position, glm::vec2 scale, glm::vec3 color = COLOR_GREEN);
		
		/// <summary>
		/// Draws debug quad.
		/// </summary>
		/// <param name="position">The position.</param>
		/// <param name="scale">The scale.</param>
		/// <param name="color">The color.</param>
		HOLLOW_API void DebugQuad(glm::vec3 position = glm::vec3(1.0f), glm::vec2 scale = glm::vec2(1.0f), glm::vec3 color = COLOR_GREEN);
		
		/// <summary>
		/// Draws debug axes.
		/// </summary>
		/// <param name="position">The position.</param>
		/// <param name="scale">The scale.</param>
		HOLLOW_API void DebugAxes(glm::vec3 position, glm::vec3 scale);
		
		/// <summary>
		/// Draws debug sphere.
		/// </summary>
		/// <param name="position">The position.</param>
		/// <param name="scale">The scale.</param>
		/// <param name="color">The color.</param>
		HOLLOW_API void DebugSphere(glm::vec3 position = glm::vec3(1.0f), glm::vec3 scale = glm::vec3(1.0f), glm::vec3 color = COLOR_GREEN);
		
		/// <summary>
		/// Draws debug cube.
		/// </summary>
		/// <param name="position">The position.</param>
		/// <param name="scale">The scale.</param>
		/// <param name="color">The color.</param>
		HOLLOW_API void DebugCube(glm::vec3 position = glm::vec3(1.0f), glm::vec3 scale = glm::vec3(1.0f), glm::vec3 color = COLOR_GREEN);
		
		/// <summary>
		/// Draws debug cube.
		/// </summary>
		/// <param name="modelMatrix">The model matrix.</param>
		/// <param name="color">The color.</param>
		HOLLOW_API void DebugCube(glm::mat4& modelMatrix, glm::vec3 color = COLOR_GREEN);
	};
}