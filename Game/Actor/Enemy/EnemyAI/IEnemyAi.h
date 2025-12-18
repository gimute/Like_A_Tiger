#pragma once
#include "StateMachineComponents\IStateMachine.h"
#include "Actor\YakuzaComponents\YakuzaStateMachine.h"
#include "Actor\Enemy\EnemyAI\EnemyAiFactory.h"

#include "Actor\YakuzaComponents\YakuzaType.h"
#include "Actor\Enemy\EnemyAI\EnemyAiBlackboard .h"

#include "IEnemyAiState.h"
#include "Actor\Enemy\EnemyMetaAi\YakuzaRole.h"

class EnemyAiFactory;

template<class ClassType>
class AiAutoRegister
{
public:
	AiAutoRegister(EnemyYakuzaType type)
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

	virtual ~IEnemyAi() = default;

	inline void UpdateTargetView(TargetCharacterView view) { m_targetView = view; }

	inline TargetCharacterView GetTargetView() { return m_targetView; }

	inline void SetYakuzaRole(YakuzaRole role) { m_yakuzaRole = role; }

	inline YakuzaRole GetYakuzaRole() { return m_yakuzaRole; }

	inline void SetIsInBattle(bool isInBattle) { m_isInBattle = isInBattle; }

	inline bool GetIsInBattle() { return m_isInBattle; }

	template<typename ClassName>
	inline bool IsAiNowStateClassName()
	{
		return IsNowStateClassName<ClassName>();
	}

protected:
	//戦闘中か?
	bool m_isInBattle = false;
	//次のステートを取得
	virtual IStateBase* GetNextState() override;
	//このAIが操作するStateMachine
	YakuzaStateMachine* m_hasStateMachine = nullptr;
	//ターゲットビュー
	TargetCharacterView m_targetView;
	//役割
	YakuzaRole m_yakuzaRole = en_YakuzaRole_None;
};

