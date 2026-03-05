#pragma once

//フェードアウト後の待機時間はロードマネージャーで管理する

enum LoadScreenProcees
{
	//フェードアウトしていなくて実行まで待機中
	en_ScreenStandby,
	//フェードアウト実行
	en_fadeOutExecuting,
	//フェードアウトしきって待機中
	en_fadeOutStandby,
	//フェードイン開始
	en_fadeInExecuting,
	//フェードイン完了
	en_fadeInCompletion
};

class LoadScreenSprite : public IGameObject
{

public:
	//コンストラクタ
	LoadScreenSprite();
	//デストラクタ
	~LoadScreenSprite() = default;

	//フェードアウト実行
	void FadeOutExecuting(float fadeSpeed);
	//フェードイン実行
	void FadeInExecuting();

	//フェードアウトしきったか
	inline bool IsFadeOut()
	{
		//if (m_screenProcessState == LoadScreenProcees::en_fadeOutStandby)
		//{
		//	return true;
		//}

		//return false;

		return m_isFadeOutEnd;
	}
	//フェードインしきったか
	inline bool IsFadeIn()
	{
		return m_isFadeInEnd;
	}

	//スタート関数
	bool Start() override;
	//アップデート関数
	void Update() override;
	//ドロー関数
	void Render(RenderContext& rc) override;
	
private:
	//イージング計算
	float EasingCalc();
	//ロード中に回るやるの更新
	void LoadScreenTrunUpdate();
	//スクリーンの現在の状態
	LoadScreenProcees m_screenProcessState = LoadScreenProcees::en_ScreenStandby;
	//フェード割合
	float m_fadeEasingRatio = 0.0f;
	//イージング処理用変数
	float m_fadeEasingMax = 0.0f;
	float m_fadeEasingMin = 0.0f;
	float m_fadeEasingSpeed = 0.0f;
	//フェードアウトしきっているかどうか
	bool m_isFadeOutEnd = false;
	//フェードインしきっているかどうか
	bool m_isFadeInEnd = false;
	//描画キャンバス
	std::shared_ptr<UICanvas> m_canvas;
	//ロード画面のスプライト
	std::shared_ptr<UIImage> m_loadScreenSprite;
	//ロード中に回るやつ
	std::shared_ptr<UIImage> m_loadScreenTrunSprite;
	/// <summary>
	/// flaot用線形補間
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	const float Leap(const float a, const float b, const float t)
	{
		return (1.0f - t) * a + t * b;
	}
};

