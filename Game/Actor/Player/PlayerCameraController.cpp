#include "stdafx.h"
#include "PlayerCameraController.h"

#include "Actor\Player\PlayerController.h"
#include "UI/PouseMenuManager.h"

///カメラ初期化
CameraUpdateData PlayerCameraController::EnterCamera()
{
	CameraUpdateData cameraUpdateData;

	cameraUpdateData.m_cameraNear = 1.0f;
	cameraUpdateData.m_cameraFar = 30000.0f;

	m_toCameraPos.Set(0.0f, 25.0f, -150.0f);

	return cameraUpdateData;
}

///カメラ更新
CameraUpdateData PlayerCameraController::UpdateCamera()
{
	CameraUpdateData cameraUpdateData;

	///注視点を計算
	Vector3 target = m_targetPos;
	//注視点を少し上げる
	target.y += 70.0f;

	Vector3 toCameraPosOld = m_toCameraPos;

	if (PouseMenuSceneManager::GetSceneManagerInstance()->IsPoseMenuActive())
	{
		cameraUpdateData.m_cameraPos = target + toCameraPosOld;

		cameraUpdateData.m_cameraTarget = target;

		return cameraUpdateData;
	}

	float x = m_cameraMoveAmountXY.x;

	float y = m_cameraMoveAmountXY.y;

	bool cameraUporDawnFlag = false;

	//上下カメラ移動抑制
	if (y >= 0.6f || y <= -0.6)
	{
		cameraUporDawnFlag = true;
	}

	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, 2.4f * x);
	qRot.Apply(m_toCameraPos);

	if (cameraUporDawnFlag)
	{
		Vector3 axisX;
		axisX.Cross(Vector3::AxisY, m_toCameraPos);
		axisX.Normalize();

		qRot.SetRotationDeg(axisX, -5.0f * y);
		qRot.Apply(m_toCameraPos);
	}

	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	if (toPosDir.y < -0.5f || toPosDir.y > 0.6f) {
		//カメラが上または下に向きすぎ
		m_toCameraPos = toCameraPosOld;
	}

	Vector3 calcVec = m_toCameraPos;
	calcVec.Normalize();

	//高さを25.0fに戻す処理
	if ((m_toCameraPos.y > 25 || m_toCameraPos.y < 25) &&
		!IsInputCameraAmount())
	{
		//現在のXZ距離を計算(水平面の半径)
		float lengthXZ = std::sqrt(m_toCameraPos.x * m_toCameraPos.x + m_toCameraPos.z * m_toCameraPos.z);

		//高さを25.0fに固定
		m_toCameraPos.y = 25.0f;

		//全体の目標距離
		const float targetLength = 150.0f;

		//現在の全体距離を計算
		float currentLength = std::sqrt(lengthXZ * lengthXZ + m_toCameraPos.y * m_toCameraPos.y);

		//ゼロ除算回避
		if (currentLength > 0.0001f)
		{
			float scale = targetLength / currentLength;

			//スケーリングして距離を150に調整
			m_toCameraPos.x *= scale;
			m_toCameraPos.z *= scale;
		}
	}

	//カメラ座標を計算
	Vector3 cameraPos = target + m_toCameraPos;

	cameraUpdateData.m_cameraPos = cameraPos;
	cameraUpdateData.m_cameraTarget = target;

	return cameraUpdateData;
}

///カメラ終了
void PlayerCameraController::ExitCamera()
{
	m_toCameraPos = Vector3::Zero;
	
	m_targetPos = Vector3::Zero;

	m_cameraMoveAmountXY = Vector2::Zero;
}