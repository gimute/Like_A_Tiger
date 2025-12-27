#include "stdafx.h"
#include "EnemysHpGauge.h"

#include "GameScene\UpdateOrder.h"
#include "Actor\Enemy\Enemy.h"
#include "Actor\YakuzaComponents\YakuzaCharacter.h"

#include "BattleArea\BattleAreaManager.h"

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
	BattleAreaManager::GetInstance()->RegisterOnEnterListener(
		[&](const BattleArea& area)
		{
			 m_isCreateHpUi = CreateEnemyHpUI(area.m_id);
		}
	);

	return true;
}

void EnemysHpGauge::Update()
{
	if (m_isCreateHpUi)
	{
		RemoveDeadEnemyHpUI();

		UpdateEnemyGroupeHpInfo();
	}
}

bool EnemysHpGauge::CreateEnemyHpUI(int areaId)
{
	auto& enemyInfoList = EnemyManager::GetInstance()->GetEnemyInfoList();

	EnemyInfoGroupe* existGroup = nullptr;

	for (auto& infoPtr : enemyInfoList)
	{
		if (infoPtr.m_battleAreaId == areaId)
		{
			existGroup = &infoPtr;

			break;
		}
	}

	int listSize = 0;

	if (existGroup)
	{
		listSize = existGroup->m_enemyAiInfoList.size();
	}
	else
	{
		return false;
	}

	//HPリストを初期化
	m_enemyHpList.clear();

	//HPの生成
	for (int hpNo = 0; hpNo < listSize; ++hpNo)
	{
		EnemyHpInfo newInfo;
		//HPUIの初期化
		HPGauge* newHpUi = NewGO<HPGauge>(UpdateOrder::UI, "enemy");

		newHpUi->Init();

		newHpUi->SetVisible(false);

		Vector3 hpUiPosition = EnemyHpGaugeConstant::HPUI_POSITION;

		hpUiPosition.y += hpNo * 50.0f;

		newHpUi->SetPosition(hpUiPosition);

		newHpUi->SetScale({ 0.2f,0.3f,0.0 });
		//表示名Renderの初期化
		//表示可能に設定する
		newHpUi->InitUseName();

		Vector3 fontPosition = hpUiPosition;

		fontPosition.x += 110.0f;
		fontPosition.y += -50.0f;
		fontPosition.y += 10.0f * hpNo;

		newHpUi->SetNamePosition(fontPosition);

		newHpUi->SetNameScale(Vector3{ 0.5f,0.5f,0.0 });

		newHpUi->SetTextDraw(false);

		//情報系初期化
		newInfo.m_hpGaugePtr = newHpUi;

		m_enemyHpList.push_back(newInfo);
	}

	//処理するグループの敵のリスト
	auto& groupeEnemyList = existGroup->m_enemyAiInfoList;

	//生成したHPのリストに敵の情報を設定
	for (int HpNo = 0; HpNo < m_enemyHpList.size(); ++HpNo)
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

			//削除だと生成しなおさないといけないので別案を考える

			DeleteGO(deleteUi);

			it = m_enemyHpList.erase(it);
		}
		else
		{
			it++;
		}

		//全てのHPが削除されたら
		if (m_enemyHpList.empty())
		{
			//フラグをfalseに
			m_isCreateHpUi = false;
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