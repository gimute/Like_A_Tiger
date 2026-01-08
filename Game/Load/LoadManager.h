#pragma once

#include "Load\LoadScreenSprite.h"

enum LoadManagerProcees
{
	//ロード待機
	en_loadStandby,
	//ロードフェードアウト実行
	en_loadFadeOutExecuting,
	//暗転待機
	en_loadFadeOutWait,
	//ロードフェードイン実行
	en_loadFadeInExecuting,
	//ロード完了
	en_loadCompletion
};

class LoadManager 
{
private:
	//シングルトンパターンを採用
	//インスタンス
	static LoadManager* m_instance;
	//コンストラクタ
	LoadManager() = default;
	//コピー禁止
	LoadManager(const LoadManager&) = delete;
	//代入禁止
	LoadManager& operator=(const LoadManager) = delete;
public:
	//デストラクタ
	~LoadManager() = default;
	//インスタンス取得
	static LoadManager* GetInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new LoadManager;
		}

		return m_instance;
	}

	//ロード開始処理
	void LoadStart(float fadeInWaitTime);
	//ロード終了処理
	void LoadEnd();

	//初期化
	void InitLoadManager();
	//更新
	void Update();

	//ロードの画面が暗転しきったかどうか
	inline bool LoadFadeOutEnd()
	{
		if (m_loadManagerProcees != LoadManagerProcees::en_loadFadeOutWait)
		{
			return false;
		}

		return true;
	}
	//ロードの画面が明けきったかどうか
	inline bool LoadFadeInEnd()
	{
		if (m_loadManagerProcees != LoadManagerProcees::en_loadStandby)
		{
			return false;
		}

		return true;
	}
private:
	//フェードアウト待機時間
	float m_fadeInWaitTime = 0.0f;
	//フェードイン実行許可
	bool m_fadeInExecutingFlag = false;
	//ロード画面スプライト
	LoadScreenSprite* m_loadScreenSprite = nullptr;
	//ロードプロセス
	LoadManagerProcees m_loadManagerProcees = LoadManagerProcees::en_loadStandby;
};

