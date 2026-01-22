#pragma once
#include "Actor\YakuzaComponents\YakuzaType.h"
#include "Actor\Enemy\EnemyFactory.h"
#include "Actor\Enemy\EnemyAI\EnemyAiFactory.h"
#include "Actor\Character.h"

#include "GameScene\UpdateOrder.h"

//シングルトンパターン

class Enemy;
class EnemyMetaAi;
class EnemySystem;

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
	//戦闘エリアID
	int m_battleAreaId = -1;
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
	//このグループのID
	int m_groupeId = -1;
	//戦闘エリアのID
	int m_battleAreaId = -1;
	//このグループが戦闘中か
	bool isInBattle = false;
	//このグループの削除フラグ
	bool m_isDelete = false;
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
	//メタAI生成依頼
	void InitEnemyManager();
	//スポーン依頼
	void RequestSpawnEnemy(EnemyYakuzaType type,const Vector3& spawnPoint);
	//グループスポーン依頼
	void RequestSpawnEnemyGroup(int spawnNum,const Vector3& spawnPoint,bool inSpYakuza);
	//死亡処理依頼
	void RequestDeadEnemyProcces(const Enemy& deadEnemyAddress);
	//全体リセット依頼
	void RequestResetEnemysProcees();
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
	YakuzaCharacter* m_targetCharacter = nullptr;
	//ターゲットのビュー情報
	TargetCharacterView m_targetView;
	//メタAI
	EnemyMetaAi* m_enemyMetaAi = nullptr;
	//AI制御
	EnemySystem* m_enemyAiSystem = nullptr;
	//敵のIDカウンター
	int m_enemyIDCounter = 0;
	//敵のグループIDカウンター
	int m_enemyGroupIDCounter = 0;
	//外部用データセット
	std::vector<EnemyInfoGroupe> m_enemyInfoList;
	//TargetView更新
	void UpdateTargetView();
	//半径ランダム座標選定
	Vector3 GetRandomPointInRadius(const Vector3& point, float radius);
	//エネミーグループ情報更新
	void UpdateEnemyGroupe();
	//データセットを更新
	void UpdateEnemyDataSet();
	//戦闘エリアIDからどの敵を戦闘状態にするかを選択
	void EnemyGroupeBattleSet(EnemyInfoGroupe& battleInEnemyGroupe);
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
	inline void SetEnemyTargetCharacter(YakuzaCharacter* target)
	{
		m_targetCharacter = target;
	}
	inline TargetCharacterView GetTargetView()
	{
		return m_targetView;
	}
	//現在残りグループ数を表示(残り数はサイズから取得、グループ削除時に変動)
	inline int GetCurrentEnemyGroupeNum()
	{
		return m_enemyGroupList.size();
	}
	//指定IDの敵グループが戦闘状態かどうかを設定
	inline void SetEnemyGroupeInBattle(int id,bool setIs)
	{
		for (auto& groupPtr : m_enemyGroupList)
		{
			if (groupPtr.m_groupeId == id) 
			{
				groupPtr.isInBattle = setIs;
			}
		}
	}
	//指定IDが戦闘状態かどうかを取得
	inline bool GetEnemyGroupeInBattle(int id)
	{
		for (auto& groupPtr : m_enemyGroupList)
		{
			if (groupPtr.m_groupeId == id)
			{
				return groupPtr.isInBattle;
			}
		}

		return false;
	}
	//指定IDの敵グループの削除フラグを設定
	void SetEnemyGroupeDeleteFlag(int id, bool setIs)
	{
		for (auto& groupPtr : m_enemyGroupList)
		{
			if (groupPtr.m_groupeId == id)
			{
				groupPtr.m_isDelete = setIs;
			}
		}
	}
};

