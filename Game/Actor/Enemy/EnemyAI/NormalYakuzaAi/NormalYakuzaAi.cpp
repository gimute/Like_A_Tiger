#include "stdafx.h"
#include "NormalYakuzaAi.h"

//IdleState

void NormalYakuzaIdleState::OnEnter()
{

}

void NormalYakuzaIdleState::OnUpdate()
{

	m_owner->SetMoveVec(Vector3::Zero);

}

void NormalYakuzaIdleState::OnExit()
{

}

//TrackingState

void NormalYakuzaTrackingState::OnEnter()
{

}

void NormalYakuzaTrackingState::OnUpdate()
{

}

void NormalYakuzaTrackingState::OnExit()
{

}

AiAutoRegister<NormalYakuzaAi> NormalYakuzaAi::aiSet{ EnemyType::en_normalYakuza };

IStateBase* NormalYakuzaAi::GetNextState()
{
	if (true)
	{
		return FindClassNameState<NormalYakuzaTrackingState>();
	}

	return FindClassNameState<NormalYakuzaIdleState>();
}

