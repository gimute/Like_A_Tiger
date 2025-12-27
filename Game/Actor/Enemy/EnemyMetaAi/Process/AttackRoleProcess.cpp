#include "stdafx.h"
#include "AttackRoleProcess.h"

#include "Actor\Enemy\EnemyAI\EnemyAiState\EnemyAiWaitingAttackState.h"
#include "Actor\Enemy\EnemyAI\EnemyAiState\EnemyAiTrackingState.h"
#include "Actor\Enemy\EnemyAI\EnemyAiState\IEnemyAttackAiState.h"

#include "Actor\Enemy\Enemy.h"
#include "Actor\YakuzaComponents\YakuzaStateMachine.h"
#include "Actor\YakuzaComponents\YakuzaStates.h"

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
		for (auto& ptr : groupePtr->FindGroup(groupePtr->m_useGroupeId)->m_enemyAiInfoList)
		{
			ptr.m_enemyAi->SetYakuzaRole(YakuzaRole::en_YakuzaRole_Wait);
		}

		return;
	}

	//グループ内の敵のリスト
	auto enemyList = groupePtr->FindGroup(groupePtr->m_useGroupeId)->m_enemyAiInfoList;

	//使用する敵のデータを取り出す
	std::vector<EnemyMemberInfo*> useEnemyInfoList;

	for (auto & infoPtr : enemyList)
	{
		//現在が死亡ステートまたはダメージステート、現在死亡済みだったら飛ばす
		if (infoPtr.m_enemy->GetYakuzaStateMachine()
			.IsGetYakuzaStateMachineNowState<YakuzaDeadState>() ||
			infoPtr.m_enemy->GetYakuzaStateMachine()
			.IsGetYakuzaStateMachineNowState<YakuzaDamageState>() ||
			infoPtr.m_enemy->GetYakuzaStateMachine()
			.IsHasCharacterDead())
		{
			continue;
		}

		useEnemyInfoList.push_back(&infoPtr);
	}

	//もしも誰も攻撃できる状態で無ければ戻す
	if (useEnemyInfoList.empty())
	{
		return;
	}

	//スコアのリスト
	auto& scoreList = groupePtr->m_grouoeState.m_score;
	//初期化
	scoreList.clear();
	//敵の数分リサイズする
	scoreList.resize(useEnemyInfoList.size());

	Vector3 targetPos = groupePtr->m_groupeTargetPosition;
	Vector3 camFoward = groupePtr->m_camFoward;

	//リストから情報を取り出して評価を行う
	for (int enemyId = 0;enemyId < useEnemyInfoList.size(); ++enemyId)
	{
		//まずは距離計算等
		//距離ベクトル
		Vector3 distanceVec = useEnemyInfoList[enemyId]->m_enemyPosition - targetPos;
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
		sortEnemyList[i] = *useEnemyInfoList[sortIndex[i]];
	}
	
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
	auto enemyInfoList = groupePtr->FindGroup(groupePtr->m_useGroupeId)->m_enemyAiInfoList;

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

	//多分死亡した時にちょうど攻撃役に選定されるやつが出てるので、選定時に死亡しているなら飛ばす的な処理がいるかも

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