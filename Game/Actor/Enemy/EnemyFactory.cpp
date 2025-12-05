#include "stdafx.h"
#include "EnemyFactory.h"
#include "EnemyTypeSet\EnemyTypeSetFactory.h"

#include "Actor\Enemy\Enemy.h"

#include "GameScene\UpdateOrder.h"

Enemy* EnemyFactory::CreateEnemy(EnemyType type)
{
	Enemy* newEnemy = NewGO<Enemy>(UpdateOrder::Charactar,"enemy");
	//TypeSet取得
	auto typeSet = EnemyTypeSetFactory::GetInstance().Create(type);
	//アタックステートマシン初期化
	newEnemy->GetYakuzaStateMachine().InitAttackStateMachine(typeSet.get()->GetFirstAttackID(), typeSet.get()->GetFirstFinishBrowID());
	//TypeSet攻撃ステートリスト作成
	typeSet.get()->CreateActions(newEnemy->GetYakuzaStateMachine().GetAttackStateMachine());
	//モデルファイルパス登録
	newEnemy->InitEnemyModel(typeSet.get()->GetModelFilePath(), typeSet.get()->GetAnimationDataList());
	//攻撃力取得関数登録
	newEnemy->GetYakuzaStateMachine().InitGetAttackPowerFunc([&](YakuzaAttackComboStateMachine* obj) { return typeSet.get()->GetAttackPower(obj); });

	return newEnemy;
}

