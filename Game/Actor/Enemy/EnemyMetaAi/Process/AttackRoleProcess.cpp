#include "stdafx.h"
#include "AttackRoleProcess.h"

#include "Actor\Enemy\EnemyAI\EnemyAiState\EnemyAiWaitingAttackState.h"
#include "Actor\Enemy\EnemyAI\EnemyAiState\IEnemyAttackAiState.h"

void AttackRoleProcess::RoleDetermination(EnemyAiInfoGroupe* groupePtr)
{
	//リストから情報を取り出して評価を行う
	for (auto it = groupePtr->m_enemyAiInfoList.begin();
		it != groupePtr->m_enemyAiInfoList.end();
		)
	{
		//まずは距離計算等
		//距離ベクトル
		Vector3 distanceVec = m_targetPosition - it->m_enemyPosition;
		//正規化距離ベクトル
		Vector3 normalizeDistanceVec = distanceVec;
		normalizeDistanceVec.Normalize();
		//カメラの正面にいる敵には高スコア
		float dot = Dot(m_cameraFoward, normalizeDistanceVec);
		btClamp(dot, -1.0f, 1.0f);
		//スコア加算
		it->m_attackRoleScore += dot;

		//敵の現在の行動によってスコアを変動させる
		//攻撃待機中ならスコアアップ
		if (it->m_enemyAi->IsAiNowStateClassName<EnemyAiWaitingAttackState>())
		{
			it->m_attackRoleScore += 1.0f;
		}
		//待機中なら変動なし
		//攻撃中ならスコアダウン
		if (it->m_enemyAi->IsAiNowStateClassName<IEnemyAttackAiState>())
		{
			it->m_attackRoleScore -= 1.0f;
		}

		it++;
	}
}

bool AttackRoleProcess::IsMetaAiProces(EnemyAiInfoGroupe* groupePtr)
{
	auto enemyInfoList = groupePtr->m_enemyAiInfoList;

	bool isInBattle;

	for (auto & ptr : enemyInfoList)
	{
		if (ptr.m_isInBattle)
		{
			isInBattle = true;

			break;
		}
	}

	return isInBattle;
}
