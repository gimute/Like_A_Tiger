#pragma once
#include "stdafx.h"

#include "StateMachineComponents\IState.h"
#include "Actor\YakuzaComponents\YakuzaAttackComboStateMachine.h"
#include "Actor\Enemy\EnemyTypeSet\EnemyTypeSetFactory.h"

class EnemyTypeSetFactory;

template<class ClassType>
class TypeSetAutoRegister
{
public:
	TypeSetAutoRegister(uint32_t hash)
	{
		EnemyTypeSetFactory::GetInstance().Register(hash, []()
		{
			return std::make_unique<ClassType>();
		});
	}
};

class IEnemyTypeSet
{
public:
	//デストラクタ
	virtual ~IEnemyTypeSet() = default;
	//ステート生成
	virtual std::vector<std::unique_ptr<IStateBase>> CreateActions(YakuzaAttackComboStateMachine* useAttackStateMachine) const = 0;
};

