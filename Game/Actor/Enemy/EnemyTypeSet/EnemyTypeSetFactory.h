#pragma once

#include "Actor\Enemy\EnemyType.h"
#include "Actor\Enemy\EnemyTypeSet\IEnemyTypeSet.h"

class IEnemyTypeSet;

//登録ファクトリ

class EnemyTypeSetFactory
{
public:
	//関数命名
	using CreateFunc = std::function<std::unique_ptr<IEnemyTypeSet>()>;
	//インスタンス取得
	static EnemyTypeSetFactory& GetInstance()
	{
		static EnemyTypeSetFactory inst;
		return inst;
	}
	//登録
	void Register(uint32_t hash, CreateFunc func)
	{
		m_registry[hash] = func;
	}
	//作成
	std::unique_ptr<IEnemyTypeSet> Create(uint32_t hash, CreateFunc func)
	{
		auto it = m_registry.find(hash);
		if (it != m_registry.end())
		{
			return it->second();
		}
		return nullptr;
	}
private:
	//登録されたSetのリスト
	std::unordered_map<uint32_t, CreateFunc> m_registry;
};

