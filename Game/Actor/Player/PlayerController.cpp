#include "stdafx.h"
#include "PlayerController.h"

#include "PlayerStateMachine.h"

namespace
{
	inline bool IsInputStickL()
	{
		//左スティックの入力があるかどうかを判定
		if ((fabsf(g_pad[0]->GetLStickXF()) >= FLT_EPSILON) ||
			(fabsf(g_pad[0]->GetLStickYF()) >= FLT_EPSILON))
		{
			return true;
		}
		return false;
	}
};

bool PlayerController::Start()
{
	return true;
}

void PlayerController::Update()
{
	auto* targetStateMachine = m_controllTarget;

	//フィニッシュブロウの条件文を付けるならここ
	targetStateMachine->SetFinishBrowButtonY(g_pad[0]->IsTrigger(enButtonY));
	
	//通常攻撃の条件文を付けるならここ
	targetStateMachine->SetAttackButtonB(g_pad[0]->IsTrigger(enButtonB));

	//スウェイの条件文付けるならここ
	targetStateMachine->SetSwayMoveButtonA(g_pad[0]->IsTrigger(enButtonA));

	//ガードの条件文付けるならここ
	targetStateMachine->SetDefenseButtonLTandRT(
		g_pad[0]->IsPress(enButtonLB1) ||
		g_pad[0]->IsPress(enButtonRB1)
	);
		
	//Lスティックの入力があれば
	if (IsInputStickL())
	{
		targetStateMachine->SetPlayerMoveVec(GetStickL());
	}
	//Lスティックの入力量を設定
	targetStateMachine->SetStickAmount(GetStickL().Length());
}

Vector3 PlayerController::GetStickL() const 
{
	//左スティックの入力量を取得
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	//カメラの前方向と右方向のベクトルを取得
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();

	//y方向に移動しない
	forward.y = 0.0f;
	right.y = 0.0f;

	//左スティックの入力量を加算
	right *= stickL.x;
	forward *= stickL.y;

	Vector3 direction = right + forward;
	//正規化
	direction.Normalize();

	return direction;
}

