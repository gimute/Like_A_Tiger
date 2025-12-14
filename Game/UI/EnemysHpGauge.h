#pragma once

#include "HPGauge.h"

#include "Actor\Enemy\EnemyManager.h"

struct EnemyHpInfo
{
	//hpゲージのポインタ
	HPGauge* m_hpGaugePtr = nullptr;
	//処理中のエネミーのポインタ
	Enemy* m_proccesEnemyPtr = nullptr;
	//敵の名前バッファ
	wchar_t m_enemyNameBuffer[256];
	//HPUIが保持しているの敵のHP
	int m_hasEnemyHp = 0;
};

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
private:
	//先頭中の敵集団を探す
	bool SearchInBattleGroupe();
	//戦闘中の敵HP情報を更新
	void UpdateEnemyGroupeHpInfo();
private:
	//処理中のエネミーグループ
	EnemyInfoGroupe* m_proccesEnemyGroupe = nullptr;
	//エネミーのHP情報のリスト
	std::vector<EnemyHpInfo> m_enemyHpList;
};

