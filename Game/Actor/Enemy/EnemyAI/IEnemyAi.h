#pragma once
#include "StateMachineComponents\IStateMachine.h"
#include "Actor\YakuzaComponents\YakuzaStateMachine.h"
#include "Actor\Enemy\EnemyAI\EnemyAiFactory.h"

#include "Actor\Enemy\EnemyType.h"

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

protected:
	//次のステートを取得
	virtual IStateBase* GetNextState() override;
	//このAIが操作するStateMachine
	YakuzaStateMachine* m_hasStateMachine = nullptr;
};

