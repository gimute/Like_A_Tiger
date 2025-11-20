#pragma once
#include "Actor\Enemy\EnemyType.h"
#include "Actor\Enemy\EnemyFactory.h"
#include "Actor\Enemy\EnemyAI\EnemyAiFactory.h"
#include "Actor\Character.h"

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
	//更新
	void Update();
private:
	//エネミーのファクトリー
	EnemyFactory m_enemyFactory;
	//AIのファクトリー
	EnemyAiFactory m_enemyAiFactory;
	//エネミーのリスト
	std::vector<Enemy*> m_enemyList;
	//AIのリスト
	std::vector<std::unique_ptr<IEnemyAi>> m_aiList;
	//敵のターゲットのCharactar
	Character* m_targetCharacter = nullptr;

	TargetCharacterView m_targetView;

	//TargetView更新
	void UpdateTargetView();
public:
	//aiのリストを取得
	inline std::vector<std::unique_ptr<IEnemyAi>>& GetAiList()
	{
		return m_aiList;
	}
	//敵のターゲットを設定
	inline void SetEnemyTargetCharacter(Character* target)
	{
		m_targetCharacter = target;
	}
	inline TargetCharacterView GetTargetView()
	{
		return m_targetView;
	}
};

