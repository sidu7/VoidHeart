#pragma once
#include "Hollow/Core/Core.h"

namespace Hollow
{
	/// <summary>
	/// Class BSplineCurve.
	/// </summary>
	class BSplineCurve
	{
	private:
		BSplineCurve();
	public:
		/// <summary>
		/// Gets the point on curve.
		/// </summary>
		/// <param name="t">The parameter on the curve.</param>
		/// <param name="controlPointMatrix">The control point matrix.</param>
		/// <returns>Point on the curve glm.vec4.</returns>
		HOLLOW_API  static glm::vec4 GetPointOnCurve(float t, glm::mat4& controlPointMatrix);
		/// <summary>
		/// Gets the derivative of point on curve.
		/// </summary>
		/// <param name="t">The parameter on the curve.</param>
		/// <param name="controlPointMatrix">The control point matrix.</param>
		/// <returns>Derivate of the point on the curve glm.vec4.</returns>
		HOLLOW_API  static glm::vec4 GetDerivativeOfPointOnCurve(float t, glm::mat4& controlPointMatrix);

	private:
		static BSplineCurve instance;
		glm::mat4 mCurveMatrix;
	};
}