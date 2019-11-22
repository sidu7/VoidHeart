#include <chrono>

struct Timer
{
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;

	HOLLOW_API Timer()
	{
		start = std::chrono::high_resolution_clock::now();
	}

	HOLLOW_API ~Timer()
	{
		end = std::chrono::high_resolution_clock::now();
		duration = end - start;

		float ms = duration.count() * 1000.0f;
		HW_CORE_TRACE("Execution Time: {0} ms",ms);
	}
};