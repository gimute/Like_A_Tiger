#pragma once
#include "Actor\Enemy\EnemyType.h"
#include "Actor\Enemy\EnemyFactory.h"
#include "Actor\Enemy\EnemyAI\EnemyAiFactory.h"

//シングルトンパターン

class Enemy;

class EnemyManager
{
private:
	//シングルトーンパターンを採用するため、コンストラクタをprivateにする
	//インスタンス
	static EnemyManager* m_instance;
	//コンストラクタ
	EnemyManager() = default;
	//コピー禁止
	EnemyManager(const EnemyManager&) = delete;
	//代入禁止にする
	EnemyManager& operator=(const EnemyManager) = delete;
public:
	//デストラクタ
	~EnemyManager() = default;
	//インスタンス取得
	static EnemyManager* GetInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new EnemyManager();
		}

		return m_instance;
	}
	//スポーン依頼
	void RequestSpawnEnemy(EnemyType type,const Vector3& spawnPoint);
private:
	//エネミーのファクトリー
	EnemyFactory m_enemyFactory;
	//AIのファクトリー
	EnemyAiFactory m_enemyAiFactory;
	//エネミーのリスト
	std::vector<Enemy*> m_enemyList;
	//AIのリスト
	std::vector<std::unique_ptr<IEnemyAi>> m_aiList;
public:
	//aiのリストを取得
	std::vector<std::unique_ptr<IEnemyAi>>& GetAiList()
	{
		return m_aiList;
	}
};

