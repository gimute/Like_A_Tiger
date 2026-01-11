#include "stdafx.h"
#include "BattleManager.h"

#include "GameScene\UpdateOrder.h"

BattleManager* BattleManager::m_instance = nullptr;

void BattleManager::InitBattleManager()
{
	//戦闘開始時に透明壁を生成する処理を登録
	RegisterBattleStartCallBack(
		[&](const BattleStartEventInfo& eventInfo)
		{
			CreateBattleInvisibleWall(
				eventInfo.m_battleAreaCenter
			);
		}
	);
}

void BattleManager::StartBattle(BattleInfo battleInfo)
{
	//戦闘開始イベント情報作成
	BattleStartEventInfo eventInfo(
		battleInfo.m_enemyGroupeInfo,
		battleInfo.m_battleArea->m_areaCenter
	);

	//現在戦闘中の敵グループ情報を保存
	m_currentBattleEnemyGroupe = battleInfo.m_enemyGroupeInfo;

	//登録されているコールバックを呼び出す
	for (auto& cb : m_battleStartCallBackList)
	{
		cb(eventInfo);
	}

	//戦闘エリアを削除
	BattleAreaManager::GetInstance()->RemoveArea(eventInfo.m_enemyGroupeInfo->m_battleAreaId);

	//現在の戦闘状態を戦闘中に変更
	m_currentBattleState = BattleState::en_InBattle;
}

void BattleManager::InBattleUpdate()
{
	//戦闘中イベント情報作成
	BattleUpdateEventInfo eventInfo;

	for (auto& cb : m_battleUpdateCallBackList)
	{
		cb(eventInfo);
	}
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
	if (m_currentBattleState == BattleState::en_InBattle)
	{
		//敵生存確認
		EnemyAliveCheck();
		//戦闘中コールバック更新
		InBattleUpdate();
		//ついでに透明壁更新
		if (m_battleInvisibleWall)
		{
			m_battleInvisibleWall->Update();
		}
	}
}

void BattleManager::EnemyAliveCheck()
{
	//現在処理中の敵グループ情報がなければ処理を抜ける
	if (!m_currentBattleEnemyGroupe)
	{
		return;
	}

	//敵グループ内の敵が全て死亡しているか確認
	if (m_currentBattleEnemyGroupe->m_enemyAiInfoList.empty())
	{
		//全て死亡しているなら戦闘終了処理を行う
		EndBattle();
		//ついでに透明壁を削除
		RemoveBattleInvisibleWall();
		//指定の敵グループに削除フラグを立てる
		EnemyManager::GetInstance()->SetEnemyGroupeDeleteFlag(
			m_currentBattleEnemyGroupe->m_groupId,true
		);

		m_currentBattleEnemyGroupe = nullptr;

		m_currentBattleState = BattleState::en_BattleEnding;
	}
}

void BattleManager::CreateBattleInvisibleWall(Vector3 centerPos)
{
	//透明壁生成処理
	m_battleInvisibleWall = NewGO<CircleInvisibleWall>(UpdateOrder::Actor, "invisibleWall");

	m_battleInvisibleWall->Create(
		centerPos,
		600.0f
	);
}

void BattleManager::RemoveBattleInvisibleWall()
{
	//透明壁削除処理
	if (m_battleInvisibleWall)
	{
		m_battleInvisibleWall->Delete();
		DeleteGO(m_battleInvisibleWall);
		m_battleInvisibleWall = nullptr;
	}
}

void BattleManager::ResetBattleManager()
{
	m_battleStartCallBackList.clear();
	m_battleEndCallBackList.clear();

	//透明壁削除処理
	if (m_battleInvisibleWall)
	{
		m_battleInvisibleWall->Delete();
		DeleteGO(m_battleInvisibleWall);
		m_battleInvisibleWall = nullptr;
	}
}