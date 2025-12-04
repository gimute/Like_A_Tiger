#include "stdafx.h"
#include "PlayerController.h"

#include "Actor\Player\Player.h"
#include "Camera\ICameraController.h"
#include "Actor\Player\PlayerCameraController.h"
#include "Actor\YakuzaComponents\YakuzaStates.h"

namespace{
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

	inline bool IsInputStickR()
	{
		//右スティックの入力があるかどうかを判定
		if ((fabsf(g_pad[0]->GetRStickXF()) >= FLT_EPSILON) ||
			(fabsf(g_pad[0]->GetRStickYF()) >= FLT_EPSILON))
		{
			return true;
		}
		return false;
	}
}

bool PlayerController::Start()
{
	return true;
}

void PlayerController::Update()
{
	auto* playerStateMachine = &m_player->GetYakuzaStateMachine();

	auto* cameraController = m_cameraController;

	if (!playerStateMachine || !cameraController)
	{
		return;
	}

	cameraController->SetTargetPosition(playerStateMachine->GetHasCharactarPos());

	//フィニッシュブロウの条件文を付けるならここ
	playerStateMachine->SetFinishBrowFlag(g_pad[0]->IsTrigger(enButtonY));
	
	//通常攻撃の条件文を付けるならここ
	playerStateMachine->SetAttackFlag(g_pad[0]->IsTrigger(enButtonB));

	//スウェイの条件文付けるならここ
	playerStateMachine->SetSwayFlag(g_pad[0]->IsTrigger(enButtonA));

	//ガードの条件文付けるならここ
	playerStateMachine->SetDefenseFlag(
		g_pad[0]->IsPress(enButtonLB1) ||
		g_pad[0]->IsPress(enButtonRB1)
	);
		
	//Lスティックの入力があれば
	if (IsInputStickL())
	{
		playerStateMachine->SetMoveVec(CameraControllCalc());
	}

	//Rスティックの入力量を設定
	cameraController->SetCameraMoveAmountXY(
		g_pad[0]->GetRStickXF(), 
		g_pad[0]->GetRStickYF()
	);
}

Vector3 PlayerController::CameraControllCalc()
{
	//左スティックの入力量を取得
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	//カメラの前方向と右方向ベクトルを取得
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();

	//y方向には移動しない
	forward.y = 0.0f;
	right.y = 0.0f;

	forward.Normalize();
	right.Normalize();

	right *= stickL.x;
	forward *= stickL.y;

	//移動速度にスティックの入力量を加算
	Vector3 newMoveVec = right + forward;

	//移動速度にスティックの入力量を加算
	return newMoveVec;
}

Vector3 PlayerController::GetStickR() const
{
	//右スティックの入力量を取得
	Vector3 stickR;
	stickR.x = g_pad[0]->GetRStickXF();
	stickR.y = g_pad[0]->GetRStickYF();

	return stickR;
}

