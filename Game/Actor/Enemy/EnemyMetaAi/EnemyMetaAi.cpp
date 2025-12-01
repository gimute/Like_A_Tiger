#include "stdafx.h"
#include "EnemyMetaAi.h"

#include "Actor\Character.h"
#include "Actor\Enemy\EnemyAI\EnemyAiBlackboard .h"

#include "Actor\Enemy\EnemyManager.h"
#include "Actor\Enemy\Enemy.h"

#include "Actor\Enemy\EnemyAI\IEnemyAi.h"
//フロー(上から下へ)
//データ収集（敵の状態 / 距離）
//
//状況評価（誰が攻撃に向いているかスコア化）
//
//ロール仮決定
//
//ロールの競合調整
//
//クールダウン・ローテーション反映
//
//フォーメーション調整
//
//最終ロール決定
//
//敵AIへ命令を送る

//スタート関数
bool EnemyMetaAi::Start()
{
	return true;
}

void EnemyMetaAi::Update()
{
	//1.情報収集
	EnemyAiDataCollect();
	//2.状況評価
	EnemyAiSituationEvaluation();
	//3.実行処理決定
	ProcessingDecision();
	//4.役割の仮決定
	EnemyAiRoleTentativedecision();
}

//情報収集
void EnemyMetaAi::EnemyAiDataCollect()
{
	auto& enemyGroupList = EnemyManager::GetInstance()->GetEnemyGroupList();
	auto& enemyPairList = EnemyManager::GetInstance()->GetEnemyPairList();

	if (enemyGroupList.empty())
	{
		return;
	}

	auto targetView = EnemyManager::GetInstance()->GetTargetView();
	m_targetPosition = targetView.m_targetPosition;
	m_cameraFoward = targetView.m_targetForward;

	//まずはリストをクリア
	m_enemyAiInfoGroupeList.clear();

	for (auto& group : enemyGroupList)
	{
		EnemyAiInfoGroupe infoGroup;

		for (auto& id : group.m_enemyID)
		{
			EnemyPair* pair = nullptr;

			for (auto & enemy : enemyPairList)
			{
				if (enemy.m_enemyID == id)
				{
					pair = &enemy;

					break;
				}
			}

			if (pair)
			{
				continue;
			}

			IEnemyAi* inst = pair->m_enemyAi.get();
			EnemyType type = pair->m_type;
			Vector3 enemyPos = pair->m_enemy->GetPosition();
			bool isInBattle = pair->m_enemyAi.get()->GetIsInBattle();
			bool isActive = true;
			
			EnemyMemberInfo info(
				inst,
				type,
				enemyPos,
				isInBattle,
				isActive
			);

			infoGroup.m_enemyAiInfoList.push_back(info);
		}

		//グループを追加
		m_enemyAiInfoGroupeList.push_back(infoGroup);
	}
}

//状況評価
void EnemyMetaAi::EnemyAiSituationEvaluation()
{
	
	for (auto & group : m_enemyAiInfoGroupeList)
	{
		//group.m_useMetaAI->RoleDetermination(group);
	}

	//リストから情報を取り出して評価を行う
	for (auto & group : m_enemyAiInfoGroupeList)
	{

		for (auto & enemyPtr : group.m_enemyAiInfoList)
		{
			//まずは距離計算等
			//距離ベクトル
			Vector3 distanceVec = m_targetPosition - enemyPtr.m_enemyPosition;
			//正規化距離ベクトル
			Vector3 normalizeDistanceVec = distanceVec;
			normalizeDistanceVec.Normalize();
			//カメラの正面にいる敵には高スコア
			float dot = Dot(m_cameraFoward, normalizeDistanceVec);
			btClamp(dot, -1.0f, 1.0f);
			//スコア加算
			enemyPtr.m_attackRoleScore += dot;

			//敵の現在の行動によってスコアを変動させる
			//攻撃待機中ならスコアアップ
			if (enemyPtr.m_enemyAi->IsAiNowStateClassName<EnemyAiWaitingAttackState>())
			{
				enemyPtr.m_attackRoleScore += 1.0f;
			}
			//待機中なら変動なし
			//攻撃中ならスコアダウン
			if (enemyPtr.m_enemyAi->IsAiNowStateClassName<IEnemyAttackAiState>())
			{
				enemyPtr.m_attackRoleScore -= 1.0f;
			}
		}

		//点数準に並び替える
		std::sort(
			group.m_enemyAiInfoList.begin(),
			group.m_enemyAiInfoList.begin(),
			[](const EnemyMemberInfo& a, const EnemyMemberInfo& b)
			{
				return a.m_attackRoleScore > b.m_attackRoleScore;
			}
		);
	}
}

//処理決定
void EnemyMetaAi::ProcessingDecision()
{
	//グループのリスト数繰り返す
	for (auto & group : m_enemyAiInfoGroupeList)
	{
		//処理数繰り返して条件に合致する処理を代入する
		for (auto& map : m_processList)
		{
			if (map.second.get()->IsMetaAiProces(&group))
			{
				group.m_useMetaAI = map.second.get();
			}
		}
	}
}

//役割の仮決定
void EnemyMetaAi::EnemyAiRoleTentativedecision()
{

	

}