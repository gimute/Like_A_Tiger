#include "stdafx.h"
#include "EnemysHpGauge.h"

#include "GameScene\UpdateOrder.h"
#include "Actor\Enemy\Enemy.h"
#include "Actor\YakuzaComponents\YakuzaCharacter.h"

namespace EnemyHpGaugeConstant
{
	const int ENEMY_HPUI_MAX = 4;

	//HPUIの座標
	const Vector3 HPUI_POSITION = Vector3{550.0f,-400.0f,0.0f};
	//名前表示位置の加算値
	const Vector3 HPUI_NAME_POSITION_ADDVALUE;
}

bool EnemysHpGauge::Start()
{
	//表示限界数は4体
	for (int hpNo = 0;hpNo < EnemyHpGaugeConstant::ENEMY_HPUI_MAX;++hpNo)
	{
		EnemyHpInfo newInfo;

		HPGauge* newHpUi = NewGO<HPGauge>(UpdateOrder::UI, "enemy");

		newHpUi->Init();

		newHpUi->SetVisible(true);

		Vector3 position = EnemyHpGaugeConstant::HPUI_POSITION;

		position.y += hpNo * 50.0f;
 
		newHpUi->SetPosition(position);

		newHpUi->SetScale({0.2f,0.3f,0.0});

		newInfo.m_hpGaugePtr = newHpUi;

		newInfo.m_enemyName = "";

		m_enemyHpList.push_back(newInfo);
	}

	return true;
}

void EnemysHpGauge::Update()
{
	bool isSerchInBattleGroupe = SearchInBattleGroupe();

	if (isSerchInBattleGroupe)
	{
		UpdateEnemyGroupeHpInfo();
	}

}

bool EnemysHpGauge::SearchInBattleGroupe()
{
	//もうすでに処理中ならtrue
	if (m_proccesEnemyGroupe)
	{
		return true;
	}

	auto& enemyGoupeList = EnemyManager::GetInstance()->GetEnemyInfoList();

	EnemyInfoGroupe* proccesGroupe = nullptr;

	for (auto& groupePtr : enemyGoupeList)
	{
		//バトル中のグループを探す
		if (groupePtr.m_inBattle)
		{
			proccesGroupe = &groupePtr;

			break;
		}
	}

	//見つからなかったらfalseを返す
	if (!proccesGroupe)
	{
		return false;
	}

	//見つかったら保持させる
	m_proccesEnemyGroupe = proccesGroupe;
	//敵のポインタを登録する
	//処理するグループの敵のリスト
	auto& groupeEnemyList = m_proccesEnemyGroupe->m_enemyAiInfoList;

	for (int HpNo = 0;HpNo < m_enemyHpList.size();++HpNo)
	{
		//エネミーのポインタを保持
		m_enemyHpList[HpNo].m_proccesEnemyPtr = groupeEnemyList[HpNo].m_enemy;
		//現在のHPを保持
		m_enemyHpList[HpNo].m_hasEnemyHp = m_enemyHpList[HpNo].m_proccesEnemyPtr->GetYakuzaCurrentHp();
		//HPUI側の最大HPを設定
		m_enemyHpList[HpNo].m_hpGaugePtr->SetMaxHP(m_enemyHpList[HpNo].m_proccesEnemyPtr->GetYakuzaMaxHp());
		//HPUI側のHPを設定
		m_enemyHpList[HpNo].m_hpGaugePtr->SetHP(m_enemyHpList[HpNo].m_hasEnemyHp);
	}

	return true;
}

void EnemysHpGauge::UpdateEnemyGroupeHpInfo()
{
	//敵のHPの変化を監視する
	for (auto & hpPtr : m_enemyHpList)
	{
		int currentEnemyHp = hpPtr.m_proccesEnemyPtr->GetYakuzaCurrentHp();

		if (currentEnemyHp < hpPtr.m_hasEnemyHp)
		{
			hpPtr.m_hpGaugePtr->SetHP(currentEnemyHp);

			hpPtr.m_hasEnemyHp = currentEnemyHp;
		}
	}

}

void EnemysHpGauge::Render(RenderContext& rc)
{

}