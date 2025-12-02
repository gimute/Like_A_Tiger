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

	//役割決定用スコア
	float m_roleScore = 0.0f;

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

class IMetaAiProcess;

struct EnemyAiInfoGroupe
{
	//グループID
	int m_groupId = -1;
	std::vector<EnemyMemberInfo> m_enemyAiInfoList;
	//このグループに適用するMetaAI
	IMetaAiProcess* m_useMetaAI = nullptr;
	//ターゲットの座標
	Vector3 m_groupeTargetPosition = Vector3::Zero;
	//カメラの正面方向
	Vector3 m_camFoward = Vector3::Zero;
};

