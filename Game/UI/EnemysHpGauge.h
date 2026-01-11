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
	~EnemysHpGauge();

	//スタート関数
	bool Start() override;
	//アップデート関数
	void Update() override;
	//レンダー関数
	void Render(RenderContext& rc);
private:
	//敵HPを生成する
	bool CreateEnemyHpUI(EnemyInfoGroupe* enemyGroupeInfoPtr);
	//戦闘中の敵グループを査定して、HP等を削除したりする処理
	void RemoveDeadEnemyHpUI();
	//戦闘中の敵HP情報を更新
	void UpdateEnemyGroupeHpInfo();
private:
	//エネミーのHP情報のリスト
	std::vector<EnemyHpInfo> m_enemyHpList;
	//敵HPが生成されたかどうか
	bool m_isCreateHpUi = false;
};

