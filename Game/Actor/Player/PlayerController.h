#pragma once
#include "Actor\Player\PlayerCameraController.h"

class Player;
class ICameraController;
class YakuzaStateMachine;
class Enemy;
class EnemyInfoGroupe;

enum LockOnInputDir
{
	enLockOnIn,
	enLockOnRight,
	enLockOnLeft,
	enInputNone
};

class PlayerController : public IGameObject
{
public:
	//コンストラクタ
	PlayerController() = default;

	~PlayerController() = default;

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
	//ロックオン処理
	Vector3 CameraEnemyLockOnCalc(
		const Vector3& PlayerPos,
		LockOnInputDir inputDir
	);
	
	Enemy* SearchLockOnEnemy(
		std::function<float(Enemy*)> scoringFunc
	);

	Enemy* LockOnStart(
		const Vector3& camPos,
		const Vector3& camForward
	);

	Enemy* LockOnSwitch(
		const Vector3& camPos,
		const Vector3& camRight,
		Enemy* currentEnemy,
		bool isRight
	);

	float SmoothDamp(
		float current,
		float target,
		float& velocity,
		float smoothTime,
		float dt
	);

	float m_cameraNonAssistTimer = 0.0f;

	bool m_inBattle = false;

	bool m_isLockOn = false;

	EnemyInfoGroupe* m_inBattleEnemys = nullptr;

	Enemy* m_lockOnCurrent = nullptr;

	LockOnInputDir m_lockOnInputState = LockOnInputDir::enInputNone;
};

