#pragma once
#include "StateMachineComponents\IStateMachine.h"
#include "Actor\YakuzaComponents\YakuzaStateMachine.h"
#include "Actor\Enemy\EnemyAI\EnemyAiFactory.h"

#include "Actor\Enemy\EnemyType.h"
#include "Actor\Enemy\EnemyAI\EnemyAiBlackboard .h"

#include "IEnemyAiState.h"

class EnemyAiFactory;

template<class ClassType>
class AiAutoRegister
{
public:
	AiAutoRegister(EnemyType type)
	{
		EnemyAiFactory::GetInstance().Register(type, [](YakuzaStateMachine* hasStateMachine)
		{
			return std::make_unique<ClassType>(hasStateMachine);
		});
	}
};

class IEnemyAi : public IStateMachine
{
public:

	IEnemyAi(YakuzaStateMachine* hasStateMachine) : m_hasStateMachine(hasStateMachine) {}

	inline void UpdateTargetView(TargetCharacterView view) { m_targetView = view; }

	inline TargetCharacterView GetTargetView() { return m_targetView; }

	template<typename ClassName>
	inline bool IsAiNowStateClassName()
	{
		return IsNowStateClassName<ClassName>();
	}

protected:
	//次のステートを取得
	virtual IStateBase* GetNextState() override;
	//このAIが操作するStateMachine
	YakuzaStateMachine* m_hasStateMachine = nullptr;
	//ターゲットビュー
	TargetCharacterView m_targetView;
};

