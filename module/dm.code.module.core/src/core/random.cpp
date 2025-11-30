#include "random.h"
#include <random>

namespace hdn
{
	uuid64 uuid64_generate()
	{
		// Create a random number generator
		std::random_device rd; // Seed
		std::mt19937_64 gen(rd()); // 64-bit Mersenne Twister
		std::uniform_int_distribution<uuid64> dist;

		// Generate a random 64-bit number
		return dist(gen);
	}
}
