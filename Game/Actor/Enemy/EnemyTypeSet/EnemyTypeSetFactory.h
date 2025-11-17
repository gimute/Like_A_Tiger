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
	void Register(EnemyType type, CreateFunc func)
	{
		m_registry[type] = func;
	}
	//作成
	std::unique_ptr<IEnemyTypeSet> Create(EnemyType type)
	{
		auto it = m_registry.find(type);
		if (it != m_registry.end())
		{
			return it->second();
		}
		return nullptr;
	}
private:
	//登録されたSetのリスト
	std::unordered_map<EnemyType, CreateFunc> m_registry;
};

