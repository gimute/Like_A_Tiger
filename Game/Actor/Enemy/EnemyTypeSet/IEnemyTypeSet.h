#pragma once
#include "stdafx.h"

#include "StateMachineComponents\IState.h"
#include "Actor\YakuzaComponents\YakuzaAttackComboStateMachine.h"
#include "Actor\Enemy\EnemyTypeSet\EnemyTypeSetFactory.h"

#include "Actor\Enemy\EnemyType.h"

class EnemyTypeSetFactory;

template<class ClassType>
class TypeSetAutoRegister
{
public:
	TypeSetAutoRegister(EnemyType type)
	{
		EnemyTypeSetFactory::GetInstance().Register(type, []()
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
	virtual std::unordered_map<uint32_t, std::unique_ptr<IStateBase>> CreateActions(YakuzaAttackComboStateMachine* useAttackStateMachine) const = 0;
};

