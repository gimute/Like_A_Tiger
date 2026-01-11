#pragma once
#include "Camera/ICameraController.h"
#include "Camera/CameraUpdateData.h"
#include "CRC32.h"

#define appState(name)  \
public:\
	static constexpr uint32_t ID(){ return Hash32(#name); }

class PlayerController;

class PlayerCameraController :
    public ICameraController
{
	appState(PlayerCameraController)
public:
    ///コンストラクタ
	PlayerCameraController() = default;
    ///デストラクタ
    ~PlayerCameraController() = default;
	///カメラ初期化
	CameraUpdateData EnterCamera() override;
	///カメラ更新
	CameraUpdateData UpdateCamera() override;
	///カメラ終了
    void ExitCamera() override;
private:
	//注視点から視点までのベクトル
	Vector3 m_toCameraPos = Vector3::Zero;
	//中視点位置
	Vector3 m_targetPos;
	//カメラ入力量
	Vector2 m_cameraMoveAmountXY = Vector2::Zero;

	inline bool IsInputCameraAmount()
	{
		if ((m_cameraMoveAmountXY.x >= FLT_EPSILON) ||
			 (m_cameraMoveAmountXY.y >= FLT_EPSILON))
		{
			return true;
		}

		return false;
	}
public:
	//中視点位置設定
	inline void SetTargetPosition(const Vector3& pos) { m_targetPos = pos; }
	//カメラ動作入力設定
	inline void SetCameraMoveAmountXY(float x, float y) { m_cameraMoveAmountXY.y = y; m_cameraMoveAmountXY.x = x; }
	inline void SetCameraMoveAmountXY(const Vector2& xy) { m_cameraMoveAmountXY = xy; }

};

