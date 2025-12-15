#pragma once
#include "Actor\YakuzaComponents\YakuzaType.h"
#include "Actor\Enemy\EnemyFactory.h"
#include "Actor\Enemy\EnemyAI\EnemyAiFactory.h"
#include "Actor\Character.h"

#include "GameScene\UpdateOrder.h"

//シングルトンパターン

class Enemy;
class EnemyMetaAi;

struct EnemyMemberInfo
{
	//エネミーのポインタ
	Enemy* m_enemy = nullptr;
	//エネミーのAI
	IEnemyAi* m_enemyAi = nullptr;
	//エネミーの種類
	EnemyYakuzaType m_enemyType = EnemyYakuzaType::en_normalYakuza;
	//座標
	Vector3 m_enemyPosition = Vector3::Zero;
	//敵名
	const char* m_enemyName;
	//起動中か
	bool m_isActive = false;

	EnemyMemberInfo(
		Enemy* enemy,
		IEnemyAi* enemyAi,
		EnemyYakuzaType enemyType,
		const Vector3& enemyPos,
		const char* enemyName,
		bool isActive
	)
		: m_enemy(enemy)
		, m_enemyAi(enemyAi)
		, m_enemyType(enemyType)
		, m_enemyPosition(enemyPos)
		, m_enemyName(enemyName)
		, m_isActive(isActive)
	{}

	EnemyMemberInfo() = default;
};

struct EnemyInfoGroupe
{
	//グループID
	int m_groupId = -1;
	//所属している敵のリスト
	std::vector<EnemyMemberInfo> m_enemyAiInfoList;
	//戦闘中かどうか
	bool m_inBattle = false;
};

struct EnemyPair
{
	//エネミーのポインタ
	Enemy* m_enemy = nullptr;
	//エネミーAIのポインタ
	std::unique_ptr<IEnemyAi> m_enemyAi = nullptr;
	//エネミーの種類
	EnemyYakuzaType m_type = EnemyYakuzaType::en_normalYakuza;
	//エネミーのID
	int m_enemyID = -1;
	//エネミーの名前
	const char* m_enemyName;
	//コンストラクタ
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

//エネミーの仮名
constexpr const char* EnemyTempNames[] =
{
	"A",
	"B",
	"C",
	"D"
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
	//外部用データセット
	std::vector<EnemyInfoGroupe> m_enemyInfoList;
	//TargetView更新
	void UpdateTargetView();
	//半径ランダム座標選定
	Vector3 GetRandomPointInRadius(const Vector3& point, float radius);
	//データセットを更新
	void UpdateEnemyDataSet();
public:
	inline std::vector<EnemyPair>& GetEnemyPairList()
	{
		return m_enemyPairList;
	}
	inline std::vector<EnemyGroup>& GetEnemyGroupList()
	{
		return m_enemyGroupList;
	}
	//エネミー系の作業に必要な情報を根こそぎ取得する
	inline std::vector<EnemyInfoGroupe>& GetEnemyInfoList()
	{
		return m_enemyInfoList;
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
	//指定IDの敵グループが戦闘状態かどうかを設定
	inline void SetEnemyGroupeInBattle(int id,bool setIs)
	{
		auto& groupeList = GetEnemyGroupList();

		if (groupeList.size() < id)
		{
			return;
		}

		groupeList[id].isInBattle = setIs;
	}
	//指定IDが戦闘状態かどうかを取得
	inline bool GetEnemyGroupeInBattle(int id)
	{
		auto& groupeList = GetEnemyGroupList();

		if (groupeList.size() < id)
		{
			return false;
		}

		return groupeList[id].isInBattle;
	}
};

