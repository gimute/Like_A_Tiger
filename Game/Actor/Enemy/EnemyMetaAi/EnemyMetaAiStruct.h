#pragma once
#include "stdafx.h"

#include "Actor\Enemy\EnemyAI\IEnemyAi.h"
#include "Actor\Enemy\EnemyMetaAi\IMetaAiProcess.h"
#include "Actor\Enemy\EnemyManager.h"

class Enemy;

class IMetaAiProcess;

struct MetaAiGroupState
{
	//AttackRoleProcess
	float m_attackStartTime = 0.0f;
	IEnemyAi* m_nowAttackAi = nullptr;
	std::vector<float> m_score;

	//TrakingRoleProcess
	bool m_isTrakingProcessEnd = false;

	//BattleEndNotifyProcess
	bool m_isBattleEndProcessEnd = false;
	float m_processStartTime = 0.0f;

	// 処理変更時に呼ばれる初期化
	void ResetStateForProcess()
	{
		m_attackStartTime = 0.0f;
		m_nowAttackAi = nullptr;

		m_isTrakingProcessEnd = false;
	}
};

struct MetaAiProccesInfo
{
	//管理しているグループ
	EnemyInfoGroupe* m_useGroupe = nullptr;
	//このグループに適用するMetaAI
	IMetaAiProcess* m_useMetaAI = nullptr;
	//前回仕様していた処理
	IMetaAiProcess* m_prevMetaAI = nullptr;
	//処理に必要な構造体
	MetaAiGroupState m_grouoeState;
	//ターゲットの座標
	Vector3 m_groupeTargetPosition = Vector3::Zero;
	//カメラの正面方向
	Vector3 m_camFoward = Vector3::Zero;
};

