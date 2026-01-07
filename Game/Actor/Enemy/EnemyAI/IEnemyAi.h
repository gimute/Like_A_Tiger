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

	inline void SetAiState(YakuzaAiState state) { m_aiState = state; }

	inline YakuzaAiState GetAiState() { return m_aiState; }

	inline void SetYakuzaRole(YakuzaGroupeRole role) { m_yakuzaRole = role; }

	inline YakuzaGroupeRole GetYakuzaRole() { return m_yakuzaRole; }

	inline void BattleStartEnemyAI() 
	{
		m_aiState = en_YakuzaAiState_WaitMove;

		m_isInBattle = true;
	}

	inline bool GetIsInBattle() { return m_isInBattle; }

	inline bool GetHasYakuzaCharacterIsDead() { return m_hasStateMachine->IsHasCharacterDead(); }

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
	//このAI自身のステート
	YakuzaAiState m_aiState = en_YakuzaAiState_Idle;
	//集団制御内の役割
	YakuzaGroupeRole m_yakuzaRole = en_YakuzaRole_Free;
};

