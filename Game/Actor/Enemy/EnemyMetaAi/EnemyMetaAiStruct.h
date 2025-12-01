#pragma once
#include "stdafx.h"

#include "Actor\Enemy\EnemyAI\IEnemyAi.h"
#include "Actor\Enemy\EnemyMetaAi\IMetaAiProcess.h"

struct EnemyMemberInfo
{
	//エネミーのAI
	IEnemyAi* m_enemyAi = nullptr;
	//エネミーの種類
	EnemyType m_enemyType = EnemyType::en_normalYakuza;
	//座標
	Vector3 m_enemyPosition = Vector3::Zero;
	//
	bool m_isInBattle = false;
	bool m_isActive = false;

	//攻撃役決定スコア
	float m_attackRoleScore = 0.0f;

	EnemyMemberInfo(
		IEnemyAi* enemyAi,
		EnemyType enemyType,
		const Vector3& enemyPos,
		bool isInBattle,
		bool isActive
	)
		: m_enemyAi(enemyAi)
		, m_enemyType(enemyType)
		, m_enemyPosition(enemyPos)
		, m_isInBattle(isInBattle)
		, m_isActive(isActive)
	{
	}
};

struct EnemyAiInfoGroupe
{
	std::vector<EnemyMemberInfo> m_enemyAiInfoList;
	//このグループに適用するMetaAI
	IMetaAiProcess* m_useMetaAI;
};

