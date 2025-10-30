#pragma once
#include "SpringCamera.h"

#include "Camera\CameraUpdateData.h"

class CameraEntity : public IGameObject
{
public:
	///コンストラクタ
	CameraEntity() = default;
	///デストラクタ
	~CameraEntity() = default;
	///スタート関数
	bool Start() override;

	///カメラ初期化
	void InitCamera(CameraUpdateData);

	///アップデート関数
	void Update() override;	
private:
	//ばねカメラ
	SpringCamera m_springCamera;
	
	CameraUpdateData m_cameraUpdateData;

	bool m_cameraUpdateFlag = false;

public:

	inline void SetCameraUpdateFlag(bool isCameraUpdae) { m_cameraUpdateFlag = isCameraUpdae; }

	inline bool GetCameraUpdateFlag() { return m_cameraUpdateFlag; }

	inline void CameraRefresh()
	{
		m_springCamera.Refresh();
	}

	inline void SetCameraUpdateData(const CameraUpdateData& cameraUpdateData)
	{
		m_cameraUpdateData = cameraUpdateData;
	}
};

