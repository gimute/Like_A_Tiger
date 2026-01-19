#include "stdafx.h"
#include "FastYakuzaAi.h"

AiAutoRegister<FastYakuzaAi> FastYakuzaAi::aiSet{ EnemyYakuzaType::en_fastYakuza };

IStateBase* FastYakuzaAi::GetNextState()
{
	return nullptr;
}