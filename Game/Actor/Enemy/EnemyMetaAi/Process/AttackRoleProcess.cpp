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

void AttackRoleProcess::AssignRoles(MetaAiProccesInfo* groupePtr)
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
		for (auto& ptr : groupePtr->m_useGroupe->m_enemyAiInfoList)
		{
			ptr.m_enemyAi->SetYakuzaRole(YakuzaRole::en_YakuzaRole_Wait);
		}

		return;
	}

	//グループ内の敵のリスト
	auto enemyList = groupePtr->m_useGroupe->m_enemyAiInfoList;
	//スコアのリスト
	auto& scoreList = groupePtr->m_grouoeState.m_score;
	//初期化
	scoreList.clear();
	//敵の数分リサイズする
	scoreList.resize(enemyList.size());

	Vector3 targetPos = groupePtr->m_groupeTargetPosition;
	Vector3 camFoward = groupePtr->m_camFoward;

	//リストから情報を取り出して評価を行う
	for (int enemyId = 0;enemyId < enemyList.size(); ++enemyId)
	{
		//まずは距離計算等
		//距離ベクトル
		Vector3 distanceVec = enemyList[enemyId].m_enemyPosition - targetPos;
		//正規化距離ベクトル
		Vector3 normalizeDistanceVec = distanceVec;
		normalizeDistanceVec.Normalize();
		normalizeDistanceVec;
		//カメラの正面にいる敵には高スコア
		float dot = Dot(camFoward, normalizeDistanceVec);
		//スコア加算
		scoreList[enemyId] += dot;
	}

	//スコアリストのサイズを取得
	const size_t size = scoreList.size();
	//ソート用のリストを作成
	std::vector<size_t> sortIndex(size);
	//連番を振る(始めは０)
	std::iota(sortIndex.begin(), sortIndex.end(), 0);

	//ソート用のリストをスコアリストの値でソートする
	std::sort(sortIndex.begin(), sortIndex.end(),
		[&](size_t i1, size_t i2) 
		{
			return scoreList[i1] > scoreList[i2];
		}
	);
	
	//並べ替え後のクラスを格納したいためリストを作成
	std::vector<EnemyMemberInfo> sortEnemyList(size);
	for (size_t i = 0; i < size; i++)
	{
		sortEnemyList[i] = enemyList[sortIndex[i]];
	}
	
	////点数準に並び替える
	//std::sort(
	//	enemyList.begin(),
	//	enemyList.end(),
	//	[](const EnemyMemberInfo& a, const EnemyMemberInfo& b)
	//	{
	//		return a.m_roleScore > b.m_roleScore;
	//	}
	//);

	//一番上のヤツを攻撃役に
	//auto it = groupePtr->m_useGroupe->m_enemyAiInfoList.begin();
	auto it = sortEnemyList.begin();
	it->m_enemyAi->SetYakuzaRole(YakuzaRole::en_YakuzaRole_Attack);
	//今攻撃をしているAIを保持
	groupePtr->m_grouoeState.m_nowAttackAi = it->m_enemyAi;
	//一つ進めて
	it++;
	//他のヤツを待機に
	for (auto itfor = it;itfor != sortEnemyList.end();)
	{
		itfor->m_enemyAi->SetYakuzaRole(YakuzaRole::en_YakuzaRole_Wait);

		itfor++;
	}
}

bool AttackRoleProcess::IsApplicable(MetaAiProccesInfo* groupePtr)
{
	auto enemyInfoList = groupePtr->m_useGroupe->m_enemyAiInfoList;

	if (enemyInfoList.empty())
	{
		return false;
	}

	for (auto & ptr : enemyInfoList)
	{
		if (ptr.m_enemyAi->IsAiNowStateClassName<EnemyAiWaitingAttackState>())
		{
			return true;
		}
	}

	return false;
}

bool AttackRoleProcess::IsReady(MetaAiProccesInfo* groupePtr)
{
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
		!groupePtr->m_grouoeState.m_nowAttackAi->GetHasYakuzaCharacterIsDead() &&
		!groupePtr->m_grouoeState.m_nowAttackAi->IsAiNowStateClassName<IEnemyAttackAiState>() || 
		groupePtr->m_grouoeState.m_nowAttackAi &&
		!groupePtr->m_grouoeState.m_nowAttackAi->GetHasYakuzaCharacterIsDead() &&
		groupePtr->m_grouoeState.m_nowAttackAi->GetYakuzaRole() == YakuzaRole::en_YakuzaRole_HitDamage)
	{
		groupePtr->m_grouoeState.m_attackStartTime = AttackRoleProcessConstant::ATTACK_START_TIME;

		groupePtr->m_grouoeState.m_nowAttackAi = nullptr;

		return false;
	}

	return true;
}