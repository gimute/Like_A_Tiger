#include "stdafx.h"
#include "CameraManager.h"

///インスタンス初期化
CameraManager* CameraManager::m_cameraManagerInstance = nullptr;

///カメラ更新
void CameraManager::UpdateCamera()
{
	if (!m_cameraEntity->GetCameraUpdateFlag())
	{
		return;
	}

	if (m_currentCameraController != m_nextCameraController)
	{
		//現在のカメラコントローラー終了
		if (m_currentCameraController)
		{
			m_currentCameraController->ExitCamera();
		}
		//カメラリフレッシュ
		m_cameraEntity->CameraRefresh();
		//切り替え先のカメラコントローラーを現在のカメラコントローラーに設定
		m_currentCameraController = m_nextCameraController;
		//現在のカメラコントローラー初期化
		m_cameraEntity->InitCamera(m_currentCameraController->EnterCamera());
	}
	//カメラ更新データ取得
	CameraUpdateData cameraUpdateData = m_currentCameraController->UpdateCamera();

	//カメラ実体にデータを送る
	m_cameraEntity->SetCameraUpdateData(cameraUpdateData);
}