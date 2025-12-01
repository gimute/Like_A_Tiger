#pragma once

class EnemySystem : public IGameObject
{
public:
	//コンストラクタ
	EnemySystem() = default;
	//デストラクタ
	~EnemySystem() = default;

	//スタート関数
	bool Start() override;
	//アップデート関数
	void Update() override;

	//全ての敵AIの更新
	void AllAiUpdate();
};

