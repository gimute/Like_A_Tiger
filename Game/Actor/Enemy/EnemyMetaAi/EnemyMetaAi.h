#pragma once
#include "Actor\YakuzaComponents\YakuzaType.h"
#include "Actor\Enemy\EnemyMetaAi\EnemyMetaAiStruct.h"

#include "Actor\Enemy\EnemyMetaAi\Process\AttackRoleProcess.h"
#include "Actor\Enemy\EnemyMetaAi\Process\TrakingRoleProcess.h"

class IEnemyAi;

class EnemyMetaAi : public IGameObject
{
public:
	//コンストラクタ
	EnemyMetaAi()
	{
		//処理設定
		AddProcess<AttackRoleProcess>();
		//処理設定
		AddProcess<TrakingRoleProcess>();
	}
	//デストラクタ
	~EnemyMetaAi() = default;

	//スタート関数
	bool Start() override;
	//アップデート関数
	void Update() override;
private:
	//情報収集
	void EnemyAiDataCollect();
	//状況評価
	void EnemyAiSituationEvaluation();
	//処理決定
	void ProcessingDecision();
private:
	template<typename ClassName,typename... Args>
	void AddProcess(Args&&... args)
	{
		m_processList.emplace(
			ClassName::ID(),
			std::make_unique<ClassName>(std::forward<Args>(args)...)
		);
	}
private:
	//ターゲット位置
	Vector3 m_targetPosition = Vector3::Zero;
	//ターゲット正面ベクトル
	Vector3 m_cameraFoward = Vector3::Zero;
	//情報格納リスト
	std::vector<EnemyAiInfoGroupe> m_enemyAiInfoGroupeList;
	//処理格納リスト
	std::unordered_map<uint32_t, std::unique_ptr<IMetaAiProcess>> m_processList;
};

