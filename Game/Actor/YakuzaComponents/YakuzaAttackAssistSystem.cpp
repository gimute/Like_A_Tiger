#include "stdafx.h"
#include "YakuzaAttackAssistSystem.h"

#include "Actor\YakuzaComponents\YakuzaCharacter.h"
#include "Actor\Enemy\Enemy.h"
#include "Actor\Enemy\EnemyManager.h"


//インスタンス初期化
YakuzaAttackAssistSystem* YakuzaAttackAssistSystem::m_instance = nullptr;

void YakuzaAttackAssistSystem::InitAttackAssistSystem(YakuzaCharacter* playerYakuzaCharacter)
{
	//プレイヤーのポインタを格納
	m_playerYakuzaCharacterPtr = playerYakuzaCharacter;
}

YakuzaCharacter* YakuzaAttackAssistSystem::GetPlayerNearEnemyPosition(const TargetingParam& param)
{
	//一番近い敵ポインタ
	YakuzaCharacter* bestEnemy = nullptr;
	//一番近い敵スコア
	float bestScore = -FLT_MAX;
	//敵のリストを取得
	auto& enemyGroupeList = EnemyManager::GetInstance()->GetEnemyInfoList();
	//プレイヤーの座標
	Vector3 pPosition = m_playerYakuzaCharacterPtr->GetPosition();
	//プレイヤーの正面ベクトル
	Vector3 pFoward = m_playerYakuzaCharacterPtr->GetForward();
	//最大範囲の2乗
	float maxDistanceSq = param.m_maxDistance * param.m_maxDistance;

	for (auto& groupPtr : enemyGroupeList)
	{
		//戦闘状態で無いグループなら処理を飛ばす
		if (!groupPtr.m_inBattle)
		{
			continue;
		}

		//敵グループ内敵リストを取得
		auto& enemyList = groupPtr.m_enemyAiInfoList;

		for (auto& enemyInfoPtr : enemyList)
		{
			YakuzaCharacter* enemy = enemyInfoPtr.m_enemy;

			//死亡しているなら処理を飛ばす
			if (enemy->IsDead() || !enemy)
			{
				continue;
			}

			//プレイヤーからエネミーへのベクトルを計算
			Vector3 toEnemy = enemy->GetPosition() - pPosition;
			//ベクトルから長さの2乗を計算
			float distSq = toEnemy.LengthSq();

			//距離制限
			if (distSq > maxDistanceSq)
			{
				continue;
			}

			//正規化
			Vector3 toEnemyN = toEnemy;
			toEnemyN.Normalize();

			//正面判定
			float dot = toEnemyN.Dot(pFoward);

			//視野角判定
			if (dot < param.m_fovCos)
			{
				continue;
			}

			//スコア計算
			float distanceScore = 1.0f - (distSq / maxDistanceSq);

			//前を向いているほど高得点にする
			float fowardScore = dot;

			//トータル計算
			float totalScore =
				distanceScore * param.m_distanceWeight +
				fowardScore * param.m_forwardWeight;

			if (totalScore > bestScore)
			{
				bestScore = totalScore;
				bestEnemy = enemy;
			}
		}
	}

	return bestEnemy;
}