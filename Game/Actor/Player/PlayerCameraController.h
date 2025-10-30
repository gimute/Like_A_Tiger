#pragma once
#include "Camera/ICameraController.h"
#include "Camera/CameraUpdateData.h"
#include "CRC32.h"

#define appState(name)  \
public:\
	static constexpr uint32_t ID(){ return Hash32(#name); }

class Player;
class PlayerController;

class PlayerCameraController :
    public ICameraController
{
	appState(PlayerCameraController)
public:
    ///コンストラクタ
	PlayerCameraController(Player* player) : m_player(nullptr)
	{ 
		m_player = player; 
	}
    ///デストラクタ
    ~PlayerCameraController() = default;
	///カメラ初期化
	CameraUpdateData EnterCamera() override;
	///カメラ更新
	CameraUpdateData UpdateCamera() override;
	///カメラ終了
    void ExitCamera() override;
private:
	///プレイヤーへのポインタ
	Player* m_player = nullptr;
	//注視点から視点までのベクトル
	Vector3 m_toCameraPos = Vector3::Zero;
};

