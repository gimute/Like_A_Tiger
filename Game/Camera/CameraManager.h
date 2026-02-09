#pragma once
#include "CRC32.h"

#include "Camera\CameraUpdateData.h"
#include "Camera\ICameraController.h"
#include "Camera\CameraEntity.h"

#include "Actor\Player\PlayerCameraController.h"

class CameraManager
{
private:
	///シングルトーンパターン採用
	///インスタンス
	static CameraManager* m_cameraManagerInstance;
	///コンストラクタ
	CameraManager()
	{
		AddCameraController<PlayerCameraController>();
	}
	///コピー禁止
	CameraManager(const CameraManager&) = delete;
	///代入禁止
	CameraManager& operator=(const CameraManager&) = delete;
public:
	///デストラクタ
	~CameraManager() = default;
	///インスタンス作成または取得
	static CameraManager* GetCameraManagerInstance()
	{
		if (m_cameraManagerInstance == nullptr)
		{
			m_cameraManagerInstance = new CameraManager();
		}

		return m_cameraManagerInstance;
	}
	///カメラ作成
	template<typename InitControllClass, typename... Args>
	inline void CreateCamera(Args&&... args)
	{
		//カメラ生成
		m_cameraEntity = NewGO<CameraEntity>(0, "camera");

		//切り替え先のカメラコントローラーに設定
		m_nextCameraController = m_cameraControllerList[InitControllClass::ID()].get();
	}
	///カメラ削除
	void DeleteCamera()
	{
		if (m_cameraEntity)
		{
			DeleteGO(m_cameraEntity);
		}

		m_currentCameraController->ExitCamera();
		m_currentCameraController = nullptr;
	}
	///カメラコントローラー登録
	template<typename ControllClass,typename... Args>
	inline void AddCameraController(Args&&... args)
	{
		//カメラコントローラーリストに登録
		m_cameraControllerList.emplace(
			ControllClass::ID(),
			std::make_unique<ControllClass>(std::forward<Args>(args)...)
		);
	}
	///カメラ取得
	template<typename ControllClass>
	inline ICameraController* GetCameraController()
	{
		uint32_t id = ControllClass::ID();
		const auto& it = m_cameraControllerList.find(id);
		if (it == m_cameraControllerList.end())
		{
			//念のため
			return nullptr;
		}
		return it->second.get();
	}
	///切り替え先カメラコントローラー設定
	template<typename ControllClass>
	inline void SetNextCameraController()
	{
		m_nextCameraController = m_cameraControllerList[ControllClass::ID()].get();
	}
	///カメラ更新
	void UpdateCamera();
private:
	///カメラコントローラーリスト
	std::unordered_map<uint32_t,std::unique_ptr<ICameraController>> m_cameraControllerList;
	///切り替え先のカメラコントローラー
	ICameraController* m_nextCameraController = nullptr;
	///現在のカメラコントローラー
	ICameraController* m_currentCameraController = nullptr;
	///カメラ実体
	CameraEntity* m_cameraEntity = nullptr;
};

