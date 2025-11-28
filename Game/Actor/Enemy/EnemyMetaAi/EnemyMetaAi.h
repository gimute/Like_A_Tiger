#pragma once

struct EnemyAiInfo
{
	IEnemyAi* m_enemyAi = nullptr;
	Vector3 m_enemyPosition = Vector3::Zero;
	Vector3 m_targetPosition = Vector3::Zero;
	bool m_isActive = false;

	EnemyAiInfo(
		IEnemyAi* enemyAi,
		const Vector3& enemyPos,
		const Vector3& targetPos,
		bool isActive
	) 		
		: m_enemyAi(enemyAi)
		, m_enemyPosition(enemyPos)
		, m_targetPosition(targetPos)
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
private:
	//情報格納リスト
	std::vector<EnemyAiInfo> m_enemyAiInfoList;
};

