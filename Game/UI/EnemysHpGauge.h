#pragma once

class EnemysHpGauge : public IGameObject
{
public:
	//コンストラクタ
	EnemysHpGauge() = default;
	//デストラクタ
	~EnemysHpGauge() = default;

	//スタート関数
	bool Start() override;
	//アップデート関数
	void Update() override;
	//レンダー関数
	void Render(RenderContext& rc);
};

