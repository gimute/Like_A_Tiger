#pragma once
#include <random>

class Random
{
public:
	//取得
	static std::mt19937& Get()
	{
		static std::mt19937 mt{ std::random_device{}() };
		return mt;
	}
	//ランダム実行int
	static int Range(int min, int max)
	{
		std::uniform_int_distribution<int> dist(min, max);
		return dist(Get());
	}
	//ランダム実行float 
	static float Range(float min, float max)
	{
		std::uniform_real_distribution<float> dist(min, max);
		return dist(Get());
	}
	//ランダム実行bool
	static bool Range(float probability)
	{
		std::bernoulli_distribution dist(0.3);
		bool flag = dist(Get());
		return flag;
	}
};

