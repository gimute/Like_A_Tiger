#include "stdafx.h"
#include "BattleStartNotifyProcess.h"

void BattleStartNotifyProcess::AssignRoles(MetaAiProccesInfo* groupePtr)
{
	//エネミーのAIのリストを取得
	auto& enemyAiList = groupePtr->FindGroup(groupePtr->m_useGroupeId)->m_enemyAiInfoList;

	//AIリスト全体に処理
	for (auto& aiPtr : enemyAiList)
	{
		//戦闘中にする
		aiPtr.m_enemyAi->SetIsInBattle(true);
	}

	groupePtr->m_grouoeState.m_isBattleStartProcessEnd = true;
}

bool BattleStartNotifyProcess::IsApplicable(MetaAiProccesInfo* groupePtr)
{
	//エネミーのAIリストを取得
	auto& enemyAI = groupePtr->FindGroup(groupePtr->m_useGroupeId)->m_enemyAiInfoList;

	//リストのAIが戦闘中かどうか
	bool isAiInBattle = false;
	//取得したグループが戦闘中かどうか
	bool isGropeInBattle = false;

	//AIリスト全体を探索
	for (auto & aiPtr : enemyAI)
	{
		//エネミーのAIがすでに戦闘中だったら
		if (aiPtr.m_enemyAi->GetIsInBattle())
		{
			//このグループのAIは戦闘中
			isAiInBattle = true;
		}
	}

	if (groupePtr->FindGroup(groupePtr->m_useGroupeId)->m_inBattle)
	{
		//このグループは戦闘中
		isGropeInBattle = true;
	}

	//AIリスト全体が戦闘中ではなく、グループが戦闘中であれば
	if (isGropeInBattle &&
		!isAiInBattle)
	{
		//AIに戦闘中伝達がまだなので処理を実行する
		return true;
	}
	else
	{
		//処理を実行しない
		return false;
	}
}

bool BattleStartNotifyProcess::IsReady(MetaAiProccesInfo* groupePtr)
{
	if (groupePtr->m_grouoeState.m_isBattleStartProcessEnd)
	{
		groupePtr->m_grouoeState.m_isBattleStartProcessEnd = false;

		return false;
	}

	return true;
}