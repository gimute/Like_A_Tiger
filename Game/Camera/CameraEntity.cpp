#include "stdafx.h"
#include "CameraEntity.h"

//スタート関数
bool CameraEntity::Start()
{

	m_springCamera.Init(
		*g_camera3D,
		1000.0f,
		true,
		5.0f
	);

	m_springCamera.SetNear(1.0f);
	m_springCamera.SetFar(30000.0f);
	m_springCamera.SetDampingRate(0.5f);

	SetCameraUpdateFlag(true);

	return true;
}

void CameraEntity::InitCamera(CameraUpdateData cameraUpdateData)
{
	m_springCamera.SetPosition(cameraUpdateData.m_cameraPos);
	m_springCamera.SetTarget(cameraUpdateData.m_cameraPos);

	m_springCamera.SetNear(cameraUpdateData.m_cameraNear);
	m_springCamera.SetFar(cameraUpdateData.m_cameraFar);
}

//アップデート関数
void CameraEntity::Update()
{
	if (!m_cameraUpdateFlag)
	{
		return;
	}

	m_springCamera.SetPosition(m_cameraUpdateData.m_cameraPos);
	m_springCamera.SetTarget(m_cameraUpdateData.m_cameraTarget);

	m_springCamera.Update();
}