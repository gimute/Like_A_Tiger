#pragma once
#include "Actor\Enemy\EnemyMetaAi\EnemyMetaAiStruct.h"

class MetaAiProccesInfo;

//メタAIのステートの規定クラス
class IMetaAiProcess
{
public:
	IMetaAiProcess() = default;
	~IMetaAiProcess() = default;

	//役割の決定
	virtual void AssignRoles(MetaAiProccesInfo* groupePtr) = 0;
	//この処理を適用できる条件を満たしているか実行できるかどうか
	virtual bool IsApplicable(MetaAiProccesInfo* groupePtr) = 0;
	//この処理を続けて使えるかどうか
	virtual bool IsReady(MetaAiProccesInfo* groupePtr) = 0;
	
};

