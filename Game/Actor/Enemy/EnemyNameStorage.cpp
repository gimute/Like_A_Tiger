#include "stdafx.h"
#include "EnemyNameStorage.h"
#include "Random.h"

std::string EnemyNameStorage::GetName()
{
	//もしも有効な名前がストレージになかったら
	if (m_availableNames.empty())
	{
		//リセットする
		Reset();
	}

	//ランダム抽選
	int index = Random::Range(
		0,
		m_availableNames.size() - 1
	);
	//名前取得
	std::string name = m_availableNames[index];

	std::swap(
		m_availableNames[index],
		m_availableNames.back()
	);

	m_availableNames.pop_back();

	return name;
}