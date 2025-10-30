#pragma once
#include "Camera\CameraUpdateData.h"

class ICameraController
{
public:
	///コンストラクタ
	ICameraController() = default;
	///デストラクタ
	virtual ~ICameraController() = default;
	///初期化
	virtual CameraUpdateData EnterCamera() = 0;
	///カメラ座標更新
	virtual CameraUpdateData UpdateCamera() = 0;
	///カメラ終了
	virtual void ExitCamera() = 0;
};

