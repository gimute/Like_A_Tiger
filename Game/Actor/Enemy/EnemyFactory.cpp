#include "stdafx.h"
#include "EnemyFactory.h"
#include "EnemyTypeSet\EnemyTypeSetFactory.h"

#include "Actor\Enemy\Enemy.h"

#include "GameScene\UpdateOrder.h"

Enemy* EnemyFactory::CreateEnemy(EnemyType type)
{
	Enemy* newEnemy = NewGO<Enemy>(UpdateOrder::Charactar,"enemy");
	//基本ステートマシン作成
	newEnemy->MakeStateMachineUniquePtr<YakuzaStateMachine>(newEnemy);
	//TypeSet取得
	auto typeSet = EnemyTypeSetFactory::GetInstance().Create(type);
	//アタックステートマシン初期化
	newEnemy->GetYakuzaStateMachine()->InitAttackStateMachine(typeSet.get()->GetFirstAttackID(),typeSet.get()->GetFirstFinishBrowID());
	//TypeSet攻撃ステートリスト作成
	auto typeSetAttackList = typeSet.get()->CreateActions(newEnemy->GetYakuzaStateMachine()->GetAttackStateMachine());
	//モデルファイルパス登録
	newEnemy->InitEnemyModel(typeSet.get()->GetModelFilePath(), typeSet.get()->GetAnimationDataList());
	//攻撃ステート登録
	for (auto it = typeSetAttackList.begin(); it != typeSetAttackList.end();)
	{
		uint32_t id = it->first;

		newEnemy->GetYakuzaStateMachine()->GetAttackStateMachine()->AddState(
			id,
			std::move(it->second.get())
		);

		it++;
	}

	return newEnemy;
}

