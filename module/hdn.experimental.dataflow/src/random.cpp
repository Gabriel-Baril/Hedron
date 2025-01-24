#include "random.h"
#include <random>

namespace hdn
{
	uuid64_t GenerateUUID64()
	{
		// Create a random number generator
		std::random_device rd; // Seed
		std::mt19937_64 gen(rd()); // 64-bit Mersenne Twister
		std::uniform_int_distribution<uuid64_t> dist;

		// Generate a random 64-bit number
		return dist(gen);
	}
}
