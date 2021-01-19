#pragma once
#include <random>

class Random
{
public:
	Random() = delete;
	~Random() = delete;

	static float next_float();
	static float next_float(double_t min, double_t max);

	static double_t next_double();
	static double_t next_double(double_t min, double_t max);
private:
	static std::default_random_engine m_rng;
	static std::uniform_real_distribution<double_t> m_realDistribution;
};