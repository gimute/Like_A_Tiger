#pragma once
#include "Actor\Enemy\EnemyManager.h"

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

	BattleInfo() = default;

	BattleInfo(
		EnemyInfoGroupe* enemyGroupeInfo
	)
		: m_enemyGroupeInfo(enemyGroupeInfo)
	{}
};

struct BattleStartEventInfo
{
	EnemyInfoGroupe* m_enemyGroupeInfo = nullptr;

	BattleStartEventInfo() = default;

	BattleStartEventInfo(
		EnemyInfoGroupe* enemyGroupeInfo
	)
		: m_enemyGroupeInfo(enemyGroupeInfo)
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

	//戦闘終了コールバック登録
	inline void RegisterBattleEndCallBack(
		BattleEndCallBack callBack
	)
	{
		m_battleEndCallBackList.push_back(std::move(callBack));
	}

	//戦闘開始
	void StartBattle(BattleInfo battleInfo);

	//戦闘終了
	void EndBattle();

	//更新
	void Update();

private:
	//敵生存確認関数
	void EnemyAliveCheck();
	//戦闘開始コールバックリスト
	std::vector<BattleStartCallBack> m_battleStartCallBackList;
	//戦闘終了コールバックリスト
	std::vector<BattleEndCallBack> m_battleEndCallBackList;

	//現在の戦闘状態
	BattleState m_currentBattleState = BattleState::en_None;
	//現在戦闘中の敵グループ情報
	EnemyInfoGroupe* m_currentBattleEnemyGroupe = nullptr;
};

