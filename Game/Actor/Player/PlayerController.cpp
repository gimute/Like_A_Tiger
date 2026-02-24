#include "stdafx.h"
#include "PlayerController.h"

#include "Actor\Player\Player.h"
#include "Camera\ICameraController.h"
#include "Actor\Player\PlayerCameraController.h"
#include "Actor\YakuzaComponents\YakuzaStates.h"
#include "UI/PouseMenuManager.h"

#include "Battle\BattleManager.h"

#include "Load\LoadManager.h"

#include "Actor\Enemy\EnemyManager.h"
#include "Actor\Enemy\Enemy.h"
#include "Actor\Enemy\EnemyAI\IEnemyAi.h"

#include "Actor\Player\PlayerCameraLockOn.h"

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

	float KAMERA_NON_ASSIST_TIME = 0.5f;
}

bool PlayerController::Start()
{
	BattleManager::GetInstance()->RegisterBattleStartCallBack(
		[&](const BattleStartEventInfo& eventInfo)
		{
			m_inBattle = true;

			PlayerCameraLockOn::GetInstance()->
				RegisterLockOnEnemyGroupe(eventInfo.m_enemyGroupeInfo);
		}
	);

	BattleManager::GetInstance()->RegisterBattleEndCallBack(
		[&](const BattleEndEventInfo& eventInfo)
		{
			m_inBattle = false;

			PlayerCameraLockOn::GetInstance()->EndLockOn();
		}
	);

	PlayerCameraLockOn::GetInstance()->InitPlayerLockOn();

	return true;
}

PlayerController::~PlayerController()
{
	PlayerCameraLockOn::GetInstance()->ResetLockOn();
}

void PlayerController::Update()
{
	auto* playerStateMachine = &m_player->GetYakuzaStateMachine();

	auto* cameraController = m_cameraController;

	if (!playerStateMachine ||
		!cameraController ||
		!LoadManager::GetInstance()->LoadFadeInEnd() ||
		PouseMenuSceneManager::GetSceneManagerInstance()->IsPoseMenuActive()
	)
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

	//掴みの条件文
	playerStateMachine->SetGrabFlag(g_pad[0]->IsTrigger(enButtonX));	

	//ロックオンの条件文
	//ロックオン入り
	if (m_inBattle)
	{
		if (g_pad[0]->IsTrigger(enButtonUp))
		{
			if (PlayerCameraLockOn::GetInstance()->IsLockOn())
			{
				PlayerCameraLockOn::GetInstance()->EndLockOn();
			}
			else
			{
				PlayerCameraLockOn::GetInstance()->StartLockOn();
			}
		}
		//対象変更 右
		else if (PlayerCameraLockOn::GetInstance()->IsLockOn() && 
			g_pad[0]->IsTrigger(enButtonRight))
		{
			PlayerCameraLockOn::GetInstance()->SwitchLockOn(true);
		}
		//対象変更 左
		else if (PlayerCameraLockOn::GetInstance()->IsLockOn() &&
			g_pad[0]->IsTrigger(enButtonLeft))
		{
			PlayerCameraLockOn::GetInstance()->SwitchLockOn(false);
		}
	}

	//ガードの条件文付けるならここ
	playerStateMachine->SetDefenseFlag(
		g_pad[0]->IsPress(enButtonLB1) ||
		g_pad[0]->IsPress(enButtonRB1)
	);
		
	//Lスティックの入力があれば
	if (IsInputStickL())
	{
		//移動入力
		playerStateMachine->SetMoveVec(CameraInMoveCalc());
	}

	//Rスティックの入力量を設定
	cameraController->SetCameraMoveAmountXY(
		CameraXFCalc(*playerStateMachine),
		g_pad[0]->GetRStickYF()
	);
}

Vector3 PlayerController::CameraInMoveCalc()
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

float PlayerController::CameraXFCalc(YakuzaStateMachine& stateMachine)
{
	//左スティックが入力されていればアシスト無し時間を初期化
	if (IsInputStickR())
	{
		m_cameraNonAssistTimer = KAMERA_NON_ASSIST_TIME;

		if (PlayerCameraLockOn::GetInstance()->IsLockOn())
		{
			PlayerCameraLockOn::GetInstance()->EndLockOn();
		}
	}
	//アシスト無し時間が0以上であれば時間をフレームタイムで減少
	//現在のRスティック値を返す
	if (m_cameraNonAssistTimer > 0.0f)
	{
		m_cameraNonAssistTimer -= g_gameTime->GetFrameDeltaTime();

		return g_pad[0]->GetRStickXF();;
	}

	if (PlayerCameraLockOn::GetInstance()->IsLockOn())
	{
		Vector3 lockDir = Vector3::Zero;

		bool isCompleteLockOn = PlayerCameraLockOn::GetInstance()->CalcCameraLockOn(
			lockDir,
			stateMachine.GetHasCharactarPos()
		);

		//もしロック計算内部でロックオンが打ち切られたら
		if (!isCompleteLockOn)
		{
			return g_pad[0]->GetRStickXF();
		}

		return CameraDirectionToFowardMoveCalc(lockDir);
	}

	//攻撃中であれば攻撃であればカメラをそちらに向ける
	if(stateMachine.GetIsAttack())
	{
		return CameraDirectionToFowardMoveCalc(stateMachine.GetHasCharactarForward());
	}

	return g_pad[0]->GetRStickXF();;
}

Vector3 PlayerController::GetStickR() const
{
	//右スティックの入力量を取得
	Vector3 stickR;
	stickR.x = g_pad[0]->GetRStickXF();
	stickR.y = g_pad[0]->GetRStickYF();

	return stickR;
}

float PlayerController::CameraDirectionToFowardMoveCalc(const Vector3& moveDir)
{
	// 方向取得
	Vector3 camForward = g_camera3D->GetForward();
	Vector3 moveForward = moveDir;

	// Y成分カット
	camForward.y = 0.0f;
	moveForward.y = 0.0f;

	// ゼロ対策
	if (camForward.LengthSq() < 0.0001f ||
		moveForward.LengthSq() < 0.0001f)
	{
		return 0.0f;
	}

	// 正規化（超重要）
	camForward.Normalize();
	moveForward.Normalize();

	//-------------------------
	// Yaw差分計算（安定版）
	//-------------------------
	float camYaw = atan2f(camForward.x, camForward.z);
	float moveYaw = atan2f(moveForward.x, moveForward.z);

	float delta = moveYaw - camYaw;

	// -π ～ π に正規化（最短）
	while (delta > DirectX::XM_PI)   delta -= DirectX::XM_2PI;
	while (delta < -DirectX::XM_PI)  delta += DirectX::XM_2PI;

	//-------------------------
	// デッドゾーン
	//-------------------------
	const float deadZone =
		DirectX::XM_PI / 180.0f * 1.0f; // 1度

	static float current = 0.0f;
	static float velocity = 0.0f;

	if (fabs(delta) < deadZone)
	{
		current = 0.0f;
		velocity = 0.0f;
		return 0.0f;
	}

	//-------------------------
	// 入力化
	//-------------------------
	const float maxAngle = DirectX::XM_PIDIV2; // 90度

	float target = delta / maxAngle;
	target = btClamped(target, -1.0f, 1.0f);

	//-------------------------
	// SmoothDamp
	//-------------------------
	const float smoothTime = 0.15f; // 調整用

	current = SmoothDamp(
		current,
		target,
		velocity,
		smoothTime,
		g_gameTime->GetFrameDeltaTime() * 10.0f);

	return current;
}

float PlayerController::SmoothDamp(
	float current,
	float target,
	float& velocity,
	float smoothTime,
	float dt)
{
	float omega = 2.0f / smoothTime;

	float x = omega * dt;
	float exp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);

	float change = current - target;
	float temp = (velocity + omega * change) * dt;

	velocity = (velocity - omega * temp) * exp;

	return target + (change + temp) * exp;
}
