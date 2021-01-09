#include "Random.h"


std::default_random_engine Random::m_rng = std::mt19937(std::_Random_device());
std::uniform_real_distribution<double_t> Random::m_realDistribution = std::uniform_real_distribution<double_t>();

double_t Random::next_double()
{
	return m_realDistribution(m_rng);
}

double_t Random::next_double(double_t min, double_t max)
{
	std::uniform_real_distribution<double_t> distribution(min, max);
	return distribution(m_rng);

}
