#pragma once
#include "Actor\Enemy\EnemyAI\IEnemyAiState.h"

#include "CRC32.h"
#include "Random.h"

#define appState(name)	\
public:\
	static constexpr uint32_t ID() { return Hash32(#name); }

enum AttackCommand
{
	en_normalAttack,
	en_finishBrow,
};

struct ComboData
{
	std::vector<AttackCommand> m_commands;
};

class IEnemyAttackAiState : public IEnemyAiState
{
	appState(IEnemyAttackAiState)
public:
	IEnemyAttackAiState(YakuzaStateMachine* hasStateMachine)
		: IEnemyAiState(hasStateMachine)
	{}

	~IEnemyAttackAiState() = default;
	//コンボ選択
	inline void ComboChoice()
	{
		int comboNum = m_attackCombos.size() - 1;

		int choiceCombo = Random::Range(0, comboNum);

		m_choiceCombo = &m_attackCombos[choiceCombo];

		m_comboIndex = 0;
	}

	inline bool GetCombo(int& choice)
	{
		if (m_comboIndex >= m_choiceCombo->size())
		{
			return false;
		}

		choice = (*m_choiceCombo)[m_comboIndex];

		++m_comboIndex;

		return true;
	}

	//ステートイン
	virtual void OnEnter() override;
	//ステートアップデート
	virtual void OnUpdate() override;
	//ステートアウト
	virtual void OnExit() override;
protected:

	std::vector<std::vector<AttackCommand>> m_attackCombos;

	std::vector<AttackCommand>* m_choiceCombo = nullptr;

	size_t m_comboIndex = 0;
};

