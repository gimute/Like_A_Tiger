#pragma once
#include "Actor\Player\PlayerCameraController.h"

class Player;
class ICameraController;
class YakuzaStateMachine;
class Enemy;
class EnemyInfoGroupe;

class PlayerController : public IGameObject
{
public:
	//コンストラクタ
	PlayerController() = default;

	~PlayerController();

	//スタート関数
	bool Start() override;
	//アップデート関数
	void Update() override;

	//プレイヤーのポインタ
	Player* m_player = nullptr;
	//プレイヤーカメラのポインタ
	PlayerCameraController* m_cameraController = nullptr;
	//プレイヤー設定
	void SetPlayer(Player* player) { m_player = player; }
	//カメラ設定
	void SetPlayerCameraController(ICameraController* controller) { m_cameraController = dynamic_cast<PlayerCameraController*>(controller); }
private:
	//カメラを考慮した移動量計算
	Vector3 CameraInMoveCalc();
	//カメラの方向計算
	float CameraXFCalc(YakuzaStateMachine& stateMachine);
	//右スティックの入力量を取得
	Vector3 GetStickR() const;
	//カメラのX値取得
	float CameraDirectionToFowardMoveCalc(const Vector3& moveDir);

	float SmoothDamp(
		float current,
		float target,
		float& velocity,
		float smoothTime,
		float dt
	);

	float m_cameraNonAssistTimer = 0.0f;

	bool m_inBattle = false;
};

