#include "stdafx.h"
#include "EnemyMetaAi.h"

#include "Actor\Enemy\EnemyManager.h"

//フロー(上から下へ)
//データ収集（敵の状態 / 距離）
//
//状況評価（誰が攻撃に向いているかスコア化）
//
//ロール仮決定
//
//ロールの競合調整
//
//クールダウン・ローテーション反映
//
//フォーメーション調整
//
//最終ロール決定
//
//敵AIへ命令を送る

//スタート関数
bool EnemyMetaAi::Start()
{
	return true;
}

void EnemyMetaAi::Update()
{
	EnemyAiDataCollect();
}

//情報収集
void EnemyMetaAi::EnemyAiDataCollect()
{
	auto& enemyList = EnemyManager::GetInstance()->GetAiList();

	for (auto it = enemyList.begin();it != enemyList.end();)
	{
		IEnemyAi* inst = it->get();
	}
}