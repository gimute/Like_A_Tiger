#pragma once
#include "stdafx.h"

///カメラ更新データ構造体
struct CameraUpdateData
{
	///カメラ座標
	Vector3 m_cameraPos = Vector3::Zero;
	///カメラ注視点座標
	Vector3 m_cameraTarget = Vector3::Front;

	float m_cameraNear = 1.0f;

	float m_cameraFar = 30000.0f;
};
