#include "stdafx.h"
#include "AttackRoleProcess.h"

#include "Actor\Enemy\EnemyAI\EnemyAiState\EnemyAiWaitingAttackState.h"
#include "Actor\Enemy\EnemyAI\EnemyAiState\EnemyAiTrackingState.h"
#include "Actor\Enemy\EnemyAI\EnemyAiState\IEnemyAttackAiState.h"

namespace AttackRoleProcessConstant
{
	//攻撃開始までの時間
	const float ATTACK_START_TIME = 5.0f;
	//攻撃終了までの時間
	const float ATTACK_END_TIME = 15.0f;
}

void AttackRoleProcess::AssignRoles(EnemyAiInfoGroupe* groupePtr)
{
	// タイマーが未セットなら初期化
	if (groupePtr->m_grouoeState.m_attackStartTime <= 0.0f)
	{
		groupePtr->m_grouoeState.m_attackStartTime = AttackRoleProcessConstant::ATTACK_START_TIME;
	}

	// タイマー更新
	groupePtr->m_grouoeState.m_attackStartTime -= g_gameTime->GetFrameDeltaTime();

	// 0以上なら処理しない
	if (groupePtr->m_grouoeState.m_attackStartTime >= 0.0f)
	{
		for (auto& ptr : groupePtr->m_enemyAiInfoList)
		{
			ptr.m_enemyAi->SetYakuzaRole(YakuzaRole::en_YakuzaRole_Wait);
		}

		return;
	}

	Vector3 targetPos = groupePtr->m_groupeTargetPosition;
	Vector3 camFoward = groupePtr->m_camFoward;

	//リストから情報を取り出して評価を行う
	for (auto it = groupePtr->m_enemyAiInfoList.begin();
		it != groupePtr->m_enemyAiInfoList.end();
		)
	{
		//まずは距離計算等
		//距離ベクトル
		Vector3 distanceVec = it->m_enemyPosition - targetPos;
		//正規化距離ベクトル
		Vector3 normalizeDistanceVec = distanceVec;
		normalizeDistanceVec.Normalize();
		normalizeDistanceVec;
		//カメラの正面にいる敵には高スコア
		float dot = Dot(camFoward, normalizeDistanceVec);
		//スコア加算
		it->m_roleScore += dot;

		it++;
	}

	//点数準に並び替える
	std::sort(
		groupePtr->m_enemyAiInfoList.begin(),
		groupePtr->m_enemyAiInfoList.end(),
		[](const EnemyMemberInfo& a, const EnemyMemberInfo& b)
		{
			return a.m_roleScore > b.m_roleScore;
		}
	);

	//一番上のヤツを攻撃役に
	auto it = groupePtr->m_enemyAiInfoList.begin();
	it->m_enemyAi->SetYakuzaRole(YakuzaRole::en_YakuzaRole_Attack);
	//今攻撃をしているAIを保持
	groupePtr->m_grouoeState.m_nowAttackAi = it->m_enemyAi;
	//一つ進めて
	it++;
	//他のヤツを待機に
	for (auto itfor = it;itfor != groupePtr->m_enemyAiInfoList.end();)
	{
		itfor->m_enemyAi->SetYakuzaRole(YakuzaRole::en_YakuzaRole_Wait);

		itfor++;
	}

	//スコアリセット
	for (auto & ptr : groupePtr->m_enemyAiInfoList)
	{
		ptr.m_roleScore = 0.0f;
	}
}

bool AttackRoleProcess::IsApplicable(EnemyAiInfoGroupe* groupePtr)
{
	auto enemyInfoList = groupePtr->m_enemyAiInfoList;

	for (auto & ptr : enemyInfoList)
	{
		if (ptr.m_enemyAi->IsAiNowStateClassName<EnemyAiWaitingAttackState>())
		{
			return true;
		}
	}

	return false;
}

bool AttackRoleProcess::IsReady(EnemyAiInfoGroupe* groupePtr)
{
	auto enemyInfoList = groupePtr->m_enemyAiInfoList;

	////このままじゃ永遠に追ってくるのでタイマー付ける
	// ちょっと難しいので検討
	//途中抜けした際にタイマーがリセットされないのに対策必要かも
	//if (m_nowAttackAi &&
	//	m_nowAttackAi->IsAiNowStateClassName<IEnemyAttackAiState>())
	//{
	//	// タイマーが未セットなら初期化
	//	if (m_attackStartTimer <= 0.0f)
	//	{
	//		m_attackStartTimer = AttackRoleProcessConstant::ATTACK_END_TIME;
	//	}

	//	// タイマー更新
	//	m_attackEndTimer -= g_gameTime->GetFrameDeltaTime();

	//	if (m_attackEndTimer <= 0.0f)
	//	{
	//		m_nowAttackAi->SetYakuzaRole(YakuzaRole::en_YakuzaRole_Wait);

	//		m_attackEndTimer = AttackRoleProcessConstant::ATTACK_END_TIME;

	//		m_nowAttackAi = nullptr;

	//		return false;
	//	}
	//}

	if (groupePtr->m_grouoeState.m_nowAttackAi &&
		!groupePtr->m_grouoeState.m_nowAttackAi->IsAiNowStateClassName<IEnemyAttackAiState>() || 
		groupePtr->m_grouoeState.m_nowAttackAi &&
		groupePtr->m_grouoeState.m_nowAttackAi->GetYakuzaRole() == YakuzaRole::en_YakuzaRole_HitDamage)
	{
		groupePtr->m_grouoeState.m_attackStartTime = AttackRoleProcessConstant::ATTACK_START_TIME;

		groupePtr->m_grouoeState.m_nowAttackAi = nullptr;

		return false;
	}

	return true;
}