#pragma once
#include "stdafx.h"

#include "StateMachineComponents\IState.h"
#include "Actor\YakuzaComponents\YakuzaAttackComboStateMachine.h"
#include "Actor\Enemy\EnemyTypeSet\EnemyTypeSetFactory.h"

#include "Actor\Enemy\EnemyType.h"
#include "Actor\YakuzaComponents\YakuzaAnimationState.h"

#include "Actor\Character.h"

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

protected:

	uint32_t m_firstAttackID = 0;

	uint32_t m_firstFinishBrowID = 0;

	const char* m_modelFilePath = nullptr;

	std::vector<Character::AnimationData> m_animationDataList;

public:
	inline uint32_t GetFirstAttackID() { return m_firstAttackID; }

	inline uint32_t GetFirstFinishBrowID() { return m_firstFinishBrowID; }

	inline const char* GetModelFilePath() { return m_modelFilePath; }

	inline std::vector<Character::AnimationData>& GetAnimationDataList() { return m_animationDataList; }
};

