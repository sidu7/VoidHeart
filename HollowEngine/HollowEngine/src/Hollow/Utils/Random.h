#pragma once
#include <random>
#include <chrono>

// Usage:
// auto randomizer = Random::Range(1, 6); // call this once
// int randomNumber = randomizer(); // call this any number of times

/// <summary>
/// Class Random.
/// </summary>
class Random
{
public:
	/// <summary>
	/// Returns a randomizer object that returns ints between a and b.
	/// </summary>
	/// <param name="a">Start of the Range.</param>
	/// <param name="b">End of the Range.</param>
	/// <returns>auto.</returns>
	HOLLOW_API static auto Range(int a, int b)
	{
		// construct a trivial random generator engine from a time-based seed:
		unsigned seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
		std::default_random_engine generator(seed);
		std::uniform_int_distribution<int> distribution(a, b);
		return std::bind(distribution, generator); // distribution(generator)
	}

	/// <summary>
	/// Returns a randomizer object that returns ints between a and b.
	/// </summary>
	/// <param name="a">Start of the Range.</param>
	/// <param name="b">End of the Range.</param>
	/// <returns>auto.</returns>
	HOLLOW_API static auto Range(float a = 0.0f, float b = 1.0f)
	{
		// construct a trivial random generator engine from a time-based seed:
		unsigned seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
		std::default_random_engine generator(seed);
		std::uniform_real_distribution<float> distribution(a, b);
		return std::bind(distribution, generator);
	}
};