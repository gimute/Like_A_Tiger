#include "stdafx.h"
#include "BattleEndNotifyProcess.h"

#include "Actor\Enemy\EnemyAI\EnemyAiState\EnemyAiIdleState.h"
#include "Actor\Enemy\EnemyManager.h"

namespace BattleEndNotifyProcessConstant
{
	//プロセス実行までの時間
	const float PROCESS_START_TIME = 3.0f;
}

void BattleEndNotifyProcess::AssignRoles(MetaAiProccesInfo* groupePtr)
{
	EnemyManager::GetInstance()->SetEnemyGroupeInBattle(groupePtr->m_useGroupe->m_groupId, false);

	groupePtr->m_grouoeState.m_isBattleEndProcessEnd = true;
}

bool BattleEndNotifyProcess::IsApplicable(MetaAiProccesInfo* groupePtr)
{
	int trakingEndEnemy = 0;
	bool isBattleEnd = false;
	auto& enemyInfoList = groupePtr->m_useGroupe->m_enemyAiInfoList;

	if (!groupePtr->m_useGroupe->m_inBattle)
	{
		return false;
	}

	//今戦闘中なのに待機中のエネミーの数を数える
	for (auto& ptr : enemyInfoList)
	{
		if (ptr.m_enemyAi->IsAiNowStateClassName<EnemyAiIdleState>())
		{
			trakingEndEnemy++;
		}
	}

	//全員待機中の状態がタイマー中続いたら
	if (trakingEndEnemy >= enemyInfoList.size())
	{
		if (ProcessStartTimer(groupePtr))
		{
			//戦闘終了と判定
			isBattleEnd = true;
		}
	}
	//状態が続かなければ
	else
	{
		//タイマーリセット
		groupePtr->m_grouoeState.m_processStartTime = 0.0f;

		//実行はしない
		return false;
	}

	if (!groupePtr->m_grouoeState.m_isTrakingProcessEnd &&
		isBattleEnd)
	{
		groupePtr->m_grouoeState.m_isBattleEndProcessEnd = false;

		return true;
	}

	return false;
}

bool BattleEndNotifyProcess::IsReady(MetaAiProccesInfo* groupePtr)
{
	if (groupePtr->m_grouoeState.m_isBattleEndProcessEnd)
	{
		return false;
	}

	return true;
}

bool BattleEndNotifyProcess::ProcessStartTimer(MetaAiProccesInfo* groupePtr)
{
	//タイマーが未セットなら初期化
	if (groupePtr->m_grouoeState.m_processStartTime <= 0.0f)
	{
		groupePtr->m_grouoeState.m_processStartTime = BattleEndNotifyProcessConstant::PROCESS_START_TIME;
	}

	//タイマー更新
	groupePtr->m_grouoeState.m_processStartTime -= g_gameTime->GetFrameDeltaTime();

	//0以上ならfalse
	if (groupePtr->m_grouoeState.m_processStartTime >= 0.0f)
	{
		return false;
	}
	else
	{
		groupePtr->m_grouoeState.m_processStartTime = 0.0f;

		return true;
	}
}