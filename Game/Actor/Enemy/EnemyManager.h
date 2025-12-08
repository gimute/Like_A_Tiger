#pragma once
#include "Actor\YakuzaComponents\YakuzaType.h"
#include "Actor\Enemy\EnemyFactory.h"
#include "Actor\Enemy\EnemyAI\EnemyAiFactory.h"
#include "Actor\Character.h"

#include "GameScene\UpdateOrder.h"

//シングルトンパターン

class Enemy;
class EnemyMetaAi;

struct EnemyPair
{
	Enemy* m_enemy = nullptr;
	std::unique_ptr<IEnemyAi> m_enemyAi = nullptr;
	EnemyYakuzaType m_type = EnemyYakuzaType::en_normalYakuza;
	int m_enemyID = -1;

	EnemyPair(Enemy* enemy,IEnemyAi* enemyAi,EnemyYakuzaType type)
		:m_enemy(enemy)
		,m_enemyAi(enemyAi)
		,m_type(type)
	{}
	EnemyPair() = default;
};

struct EnemyGroup
{
	//エネミーの１グループ
	std::vector<int> m_enemyID;
	//このグループが戦闘中か
	bool isInBattle = false;
};

class EnemyManager
{
private:
	//シングルトーンパターンを採用するため、コンストラクタをprivateにする
	//インスタンス
	static EnemyManager* m_instance;
	//コンストラクタ
	EnemyManager();
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
	void RequestSpawnEnemy(EnemyYakuzaType type,const Vector3& spawnPoint);
	//グループスポーン依頼
	void RequestSpawnEnemyGroup(int spawnNum,const Vector3& spawnPoint);
	//更新
	void Update();
private:
	//エネミーのファクトリー
	EnemyFactory m_enemyFactory;
	//AIのファクトリー
	EnemyAiFactory m_enemyAiFactory;
	//エネミーとAiのペアリスト
	std::vector<EnemyPair> m_enemyPairList;
	//エネミーのグループリスト
	std::vector<EnemyGroup> m_enemyGroupList;
	//敵のターゲットのCharactar
	Character* m_targetCharacter = nullptr;
	//ターゲットのビュー情報
	TargetCharacterView m_targetView;
	//メタAI
	EnemyMetaAi* m_enemyMetaAi = nullptr;
	//敵のIDカウンター
	int m_enemyIDCounter = 0;
	//TargetView更新
	void UpdateTargetView();
	//半径ランダム座標選定
	Vector3 GetRandomPointInRadius(const Vector3& point, float radius);
public:
	inline std::vector<EnemyPair>& GetEnemyPairList()
	{
		return m_enemyPairList;
	}
	inline std::vector<EnemyGroup>& GetEnemyGroupList()
	{
		return m_enemyGroupList;
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

