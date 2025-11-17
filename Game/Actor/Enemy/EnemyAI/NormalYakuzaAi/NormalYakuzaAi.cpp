#include "stdafx.h"
#include "NormalYakuzaAi.h"

AiAutoRegister<NormalYakuzaAi> NormalYakuzaAi::aiSet{ EnemyType::en_normalYakuza };

IStateBase* NormalYakuzaAi::GetNextState()
{
	return nullptr;
}
