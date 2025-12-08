#pragma once

#include "Actor\YakuzaComponents\YakuzaType.h"
#include "Actor\YakuzaComponents\IYakuzaTypeSet.h"

class IYakuzaTypeSet;

//登録ファクトリ

class YakuzaTypeSetFactory
{
public:
	//関数命名
	using CreateFunc = std::function<std::unique_ptr<IYakuzaTypeSet>()>;
	//インスタンス取得
	static YakuzaTypeSetFactory& GetInstance()
	{
		static YakuzaTypeSetFactory inst;
		return inst;
	}
	//その他タイプセット登録
	void Register(OthersYakuzaType type, CreateFunc func)
	{
		m_othersYakuzaTypeRegistry[type] = func;
	}
	//エネミータイプセット登録
	void Register(EnemyYakuzaType type, CreateFunc func)
	{
		m_enemyTypeSetRegistry[type] = func;
	}

	//その他タイプセット作成
	std::unique_ptr<IYakuzaTypeSet> Create(OthersYakuzaType type)
	{
		auto it = m_othersYakuzaTypeRegistry.find(type);
		if (it != m_othersYakuzaTypeRegistry.end())
		{
			return it->second();
		}
		return nullptr;
	}
	//エネミータイプセット作成
	std::unique_ptr<IYakuzaTypeSet> Create(EnemyYakuzaType type)
	{
		auto it = m_enemyTypeSetRegistry.find(type);
		if (it != m_enemyTypeSetRegistry.end())
		{
			return it->second();
		}
		return nullptr;
	}
private:
	//登録されたその他のタイプセット
	std::unordered_map<OthersYakuzaType, CreateFunc> m_othersYakuzaTypeRegistry;
	//登録されたエネミーのタイプセット
	std::unordered_map<EnemyYakuzaType, CreateFunc> m_enemyTypeSetRegistry;
	
};

