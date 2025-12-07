#pragma once
#include "stdafx.h"

#include "StateMachineComponents\IState.h"
#include "Actor\YakuzaComponents\YakuzaAttackComboStateMachine.h"
#include "Actor\YakuzaComponents\YakuzaTypeSetFactory.h"

#include "Actor\YakuzaComponents\YakuzaType.h"
#include "Actor\YakuzaComponents\YakuzaAnimationState.h"

#include "Actor\Character.h"

class YakuzaTypeSetFactory;

template<class ClassType>
class TypeSetAutoRegister
{
public:
	TypeSetAutoRegister(OthersYakuzaType type)
	{
		YakuzaTypeSetFactory::GetInstance().Register(type, []()
			{
				return std::make_unique<ClassType>();
			});
	}
	TypeSetAutoRegister(EnemyYakuzaType type)
	{
		YakuzaTypeSetFactory::GetInstance().Register(type, []()
		{
			return std::make_unique<ClassType>();
		});
	}
};

class IYakuzaTypeSet
{
public:
	//デストラクタ
	virtual ~IYakuzaTypeSet() = default;
	//ステート生成
	virtual void CreateActions(YakuzaAttackComboStateMachine* useAttackStateMachine) = 0;

protected:

	uint32_t m_firstAttackID = 0;

	uint32_t m_firstFinishBrowID = 0;

	const char* m_modelFilePath = nullptr;

	std::vector<Character::AnimationData> m_animationDataList;

	//攻撃ステートを追加＋通常攻撃かフィニッシュブロウかを選択しカウント、trueが通常攻撃、falseがフィニッシュブロウ
	template<typename ClassName>
	inline void AddAttackState(YakuzaAttackComboStateMachine* useAttackStateMachine)
	{
		useAttackStateMachine->AddState<ClassName>(useAttackStateMachine);
	}

public:
	inline uint32_t GetFirstAttackID() { return m_firstAttackID; }

	inline uint32_t GetFirstFinishBrowID() { return m_firstFinishBrowID; }

	inline const char* GetModelFilePath() { return m_modelFilePath; }

	inline std::vector<Character::AnimationData>& GetAnimationDataList() { return m_animationDataList; }

	//攻撃力取得関数
	virtual float GetAttackPower(YakuzaAttackComboStateMachine* useAttackStateMachine) = 0;
};

