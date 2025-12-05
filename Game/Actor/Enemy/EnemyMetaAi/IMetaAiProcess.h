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
	virtual void AssignRoles(EnemyAiInfoGroupe* groupePtr) = 0;
	//この処理を適用できる条件を満たしているか実行できるかどうか
	virtual bool IsApplicable(EnemyAiInfoGroupe* groupePtr) = 0;
	//この処理を続けて使えるかどうか
	virtual bool IsReady(EnemyAiInfoGroupe* groupePtr) = 0;
	
};

