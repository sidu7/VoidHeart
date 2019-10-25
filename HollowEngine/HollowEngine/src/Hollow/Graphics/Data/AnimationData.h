#pragma once

namespace Hollow
{
	struct AnimationData
	{
		std::vector<std::pair<double, glm::vec3>> mKeyPositions;
		std::vector<std::pair<double, glm::quat>> mKeyRotations;
		std::vector<std::pair<double, glm::vec3>> mKeyScalings;
		double mDuration;
		double mTicksPerSec;
	};
}
