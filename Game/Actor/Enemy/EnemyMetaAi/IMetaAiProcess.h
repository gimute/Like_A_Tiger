#pragma once
#include "Actor\Enemy\EnemyMetaAi\EnemyMetaAiStruct.h"

class EnemyAiInfoGroupe;

//メタAIのステートの規定クラス
class IMetaAiProcess
{
public:
	IMetaAiProcess() = default;
	~IMetaAiProcess() = default;

	//役割の決定
	virtual void RoleDetermination(EnemyAiInfoGroupe* groupePtr) = 0;
	//この処理が実行できるかどうか
	virtual bool IsMetaAiProces(EnemyAiInfoGroupe* groupePtr) = 0;
};

