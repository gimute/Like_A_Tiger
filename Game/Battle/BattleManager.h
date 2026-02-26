#pragma once
#include "Actor\Enemy\EnemyManager.h"
#include "Battle\BattleAreaManager.h"

#include "InvisibleWall.h"

enum class BattleState
{
	en_None,//非戦闘
	en_BattleStarting,//戦闘開始
	en_InBattle,//戦闘中
	en_BattleEnding,//戦闘終了
};

struct BattleInfo
{
	EnemyInfoGroupe* m_enemyGroupeInfo = nullptr;

	BattleArea* m_battleArea = nullptr;

	BattleInfo() = default;

	BattleInfo(
		EnemyInfoGroupe* enemyGroupeInfo,
		BattleArea* battleArea
	)
		: m_enemyGroupeInfo(enemyGroupeInfo)
		, m_battleArea(battleArea)
	{}
};

struct BattleUpdateEventInfo
{
	//今のところ何もない
};

struct BattleStartEventInfo
{
	EnemyInfoGroupe* m_enemyGroupeInfo = nullptr;

	Vector3 m_battleAreaCenter = Vector3::Zero;

	EnemyYakuzaType m_battleMainYakuza;

	BattleStartEventInfo() = default;

	BattleStartEventInfo(
		EnemyInfoGroupe* enemyGroupeInfo,
		Vector3 battleAreaCenter,
		EnemyYakuzaType type
	)
		: m_enemyGroupeInfo(enemyGroupeInfo)
		, m_battleAreaCenter(battleAreaCenter)
		, m_battleMainYakuza(type)
	{
	}
};

struct BattleEndEventInfo
{

};

class BattleManager
{
private:
	//戦闘開始コールバック関数型
	using BattleStartCallBack = std::function<void(const BattleStartEventInfo&)>;
	//戦闘中コールバック関数型
	using BattleUpdateCallBack = std::function<void(const BattleUpdateEventInfo&)>;
	//戦闘終了コールバック関数型
	using BattleEndCallBack = std::function<void(const BattleEndEventInfo&)>;
private:
	//シングルトーンパターンを採用するため、コンストラクタをprivateにする
	//インスタンス
	static BattleManager* m_instance;
	//コンストラクタ
	BattleManager() {};
	//コピー禁止
	BattleManager(const BattleManager&) = delete;
	//代入禁止にする
	BattleManager& operator=(const BattleManager) = delete;
public:
	//デストラクタ
	~BattleManager() = default;
	//インスタンス取得
	static BattleManager* GetInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new BattleManager();
		}

		return m_instance;
	}

	//戦闘開始コールバック登録
	inline void RegisterBattleStartCallBack(
		BattleStartCallBack callBack
	)
	{
		m_battleStartCallBackList.push_back(std::move(callBack));
	}

	inline void RegisterBattleUpdateCallBack(
		BattleUpdateCallBack callBack
	)
	{
		m_battleUpdateCallBackList.push_back(std::move(callBack));
	}

	//戦闘終了コールバック登録
	inline void RegisterBattleEndCallBack(
		BattleEndCallBack callBack
	)
	{
		m_battleEndCallBackList.push_back(std::move(callBack));
	}

	//戦闘マネージャー初期化
	void InitBattleManager();

	//戦闘開始
	void StartBattle(BattleInfo battleInfo);

	//戦闘終了
	void EndBattle();

	//更新
	void Update();

	//バトルマネージャーをリセット
	void ResetBattleManager();

private:
	//敵生存確認関数
	void EnemyAliveCheck();
	//戦闘中コールバック実行
	void InBattleUpdate();
	//戦闘開始時透明壁生成関数
	void CreateBattleInvisibleWall(Vector3 centerPos);
	//戦闘終了時透明壁削除関数
	void RemoveBattleInvisibleWall();
	//戦闘開始コールバックリスト
	std::vector<BattleStartCallBack> m_battleStartCallBackList;
	//戦闘中コールバックリスト
	std::vector<BattleUpdateCallBack> m_battleUpdateCallBackList;
	//戦闘終了コールバックリスト
	std::vector<BattleEndCallBack> m_battleEndCallBackList;

	//現在の戦闘状態
	BattleState m_currentBattleState = BattleState::en_None;
	//現在戦闘中の敵グループ情報
	EnemyInfoGroupe* m_currentBattleEnemyGroupe = nullptr;
	//戦闘中の透明壁
	CircleInvisibleWall* m_battleInvisibleWall = nullptr;
};

