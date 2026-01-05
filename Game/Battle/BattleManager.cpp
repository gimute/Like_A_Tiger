#include "stdafx.h"
#include "BattleManager.h"

#include "Battle\BattleAreaManager.h"

BattleManager* BattleManager::m_instance = nullptr;

void BattleManager::StartBattle(BattleInfo battleInfo)
{
	//戦闘開始イベント情報作成
	BattleStartEventInfo eventInfo(
		battleInfo.m_enemyGroupeInfo
	);

	//現在戦闘中の敵グループ情報を保存
	m_currentBattleEnemyGroupe = battleInfo.m_enemyGroupeInfo;

	//登録されているコールバックを呼び出す
	for (auto& cb : m_battleStartCallBackList)
	{
		cb(eventInfo);
	}

	//現在の戦闘状態を戦闘中に変更
	m_currentBattleState = BattleState::en_InBattle;
}

void BattleManager::EndBattle()
{
	//戦闘終了イベント情報作成
	BattleEndEventInfo eventInfo;

	for (auto& cb : m_battleEndCallBackList)
	{
		cb(eventInfo);
	}
}

void BattleManager::Update()
{
	//敵生存確認
	EnemyAliveCheck();
}

void BattleManager::EnemyAliveCheck()
{
	//現在処理中の敵グループ情報がなければ処理を抜ける
	if (!m_currentBattleEnemyGroupe)
	{
		m_currentBattleState = BattleState::en_None;

		return;
	}

	//敵グループ内の敵が全て死亡しているか確認
	if (m_currentBattleEnemyGroupe->m_enemyAiInfoList.empty())
	{
		//全て死亡しているなら戦闘終了処理を行う
		EndBattle();

		//指定の敵グループに削除フラグを立てる
		EnemyManager::GetInstance()->SetEnemyGroupeDeleteFlag(
			m_currentBattleEnemyGroupe->m_groupId,true
		);

		m_currentBattleEnemyGroupe = nullptr;

		m_currentBattleState = BattleState::en_BattleEnding;
	}
}