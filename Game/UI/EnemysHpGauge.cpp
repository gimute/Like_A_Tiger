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
		//HPUIの初期化
		HPGauge* newHpUi = NewGO<HPGauge>(UpdateOrder::UI, "enemy");

		newHpUi->Init();

		newHpUi->SetVisible(false);

		Vector3 hpUiPosition = EnemyHpGaugeConstant::HPUI_POSITION;

		hpUiPosition.y += hpNo * 50.0f;
 
		newHpUi->SetPosition(hpUiPosition);

		newHpUi->SetScale({0.2f,0.3f,0.0});
		//表示名Renderの初期化
		//表示可能に設定する
		newHpUi->InitUseName();

		Vector3 fontPosition = hpUiPosition;

		fontPosition.x += 110.0f;
		fontPosition.y += -50.0f;
		fontPosition.y += 10.0f * hpNo;

		newHpUi->SetNamePosition(fontPosition);
			
		newHpUi->SetNameScale(Vector3{0.5f,0.5f,0.0});

		newHpUi->SetTextDraw(false);

		//情報系初期化
		newInfo.m_hpGaugePtr = newHpUi;

		m_enemyHpList.push_back(newInfo);
	}

	return true;
}

void EnemysHpGauge::Update()
{
	bool isSerchInBattleGroupe = SearchInBattleGroupe();

	if (isSerchInBattleGroupe)
	{
		RemoveDeadEnemyHpUI();

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
		//HPUIに表示する敵名を設定
		int len = MultiByteToWideChar(
			CP_UTF8,
			0,
			groupeEnemyList[HpNo].m_enemyName,
			-1,
			m_enemyHpList[HpNo].m_enemyNameBuffer,
			256
		);
		//表示する
		m_enemyHpList[HpNo].m_hpGaugePtr->SetVisible(true);
		//テキストも表示する
		m_enemyHpList[HpNo].m_hpGaugePtr->SetTextDraw(true);
	}

	return true;
}

void EnemysHpGauge::RemoveDeadEnemyHpUI()
{
	//生存しているかどうかを判定
	for (auto it = m_enemyHpList.begin();it != m_enemyHpList.end();)
	{
		bool isEnemyAlive = it->m_proccesEnemyPtr->IsDead();

		if (isEnemyAlive)
		{
			HPGauge* deleteUi = it->m_hpGaugePtr;

			DeleteGO(deleteUi);

			it = m_enemyHpList.erase(it);
		}
		else
		{
			it++;
		}
	}
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

		hpPtr.m_hpGaugePtr->SetText(hpPtr.m_enemyNameBuffer);
	}

}

void EnemysHpGauge::Render(RenderContext& rc)
{
}