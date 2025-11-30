#include "stdafx.h"
#include "EnemyMetaAi.h"

#include "Actor\Character.h"
#include "Actor\Enemy\EnemyAI\EnemyAiBlackboard .h"

#include "Actor\Enemy\EnemyManager.h"
#include "Actor\Enemy\Enemy.h"

#include "Actor\Enemy\EnemyAI\IEnemyAi.h"

#include "Actor\Enemy\EnemyAI\EnemyAiState\EnemyAiWaitingAttackState.h"
#include "Actor\Enemy\EnemyAI\EnemyAiState\IEnemyAttackAiState.h"

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
}

//情報収集
void EnemyMetaAi::EnemyAiDataCollect()
{
	auto& enemyList = EnemyManager::GetInstance()->GetEnemyPairList();
	auto targetView = EnemyManager::GetInstance()->GetTargetView();

	m_targetPosition = targetView.m_targetPosition;

	m_cameraFoward = g_camera3D->GetForward();

	for (auto it = enemyList.begin();it != enemyList.end();)
	{
		IEnemyAi* inst = it->m_enemyAi.get();
		EnemyType type = it->m_type;
		Vector3 enemyPos = it->m_enemy->GetPosition();
		//削除処理とか入れる時に編集
		bool isActive = true;

		EnemyAiInfo info(
			inst,
			type,
			enemyPos,
			isActive
		);

		m_enemyAiInfoList.push_back(info);

		it++;
	}
}

//状況評価
void EnemyMetaAi::EnemyAiSituationEvaluation()
{
	//リストから情報を取り出して評価を行う
	for (auto it = m_enemyAiInfoList.begin(); 
		it != m_enemyAiInfoList.end();
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