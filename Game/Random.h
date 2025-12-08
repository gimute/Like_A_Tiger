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
	//ランダム実行Vector3
	static Vector3 Range(const Vector3& min, const Vector3& max)
	{
		Vector3 result;
		result.x = Range(min.x, max.x);
		result.y = Range(min.y, max.y);
		result.z = Range(min.z, max.z);
		return result;
	}
	//ランダム実行bool
	static bool Range(float probability)
	{
		std::bernoulli_distribution dist(0.3);
		bool flag = dist(Get());
		return flag;
	}
};

