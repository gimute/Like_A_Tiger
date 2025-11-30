#pragma once
#include "Actor\Enemy\EnemyType.h"

class IEnemyAi;

struct EnemyAiInfo
{
	IEnemyAi* m_enemyAi = nullptr;
	EnemyType m_enemyType = EnemyType::en_normalYakuza;
	Vector3 m_enemyPosition = Vector3::Zero;
	bool m_isActive = false;

	//攻撃役決定スコア
	float m_attackRoleScore = 0.0f;

	EnemyAiInfo(
		IEnemyAi* enemyAi,
		EnemyType enemyType,
		const Vector3& enemyPos,
		bool isActive
	) 		
		: m_enemyAi(enemyAi)
		, m_enemyType(enemyType)
		, m_enemyPosition(enemyPos)
		, m_isActive(isActive)
	{}
};

class EnemyMetaAi : public IGameObject
{
public:
	//コンストラクタ
	EnemyMetaAi() = default;
	//デストラクタ
	~EnemyMetaAi() = default;

	//スタート関数
	bool Start() override;
	//アップデート関数
	void Update() override;
private:
	//情報収集
	void EnemyAiDataCollect();
	//状況評か
	void EnemyAiSituationEvaluation();
private:
	//ターゲット位置
	Vector3 m_targetPosition = Vector3::Zero;
	//ターゲット正面ベクトル
	Vector3 m_cameraFoward = Vector3::Zero;
	//情報格納リスト
	std::vector<EnemyAiInfo> m_enemyAiInfoList;
};

