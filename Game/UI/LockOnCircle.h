#pragma once

class Enemy;

class LockOnCircle : public IGameObject
{
public:
	//初期化データ
	struct LockOnCircleUIInitData
	{
		const char* frameFilePath;
		int frameSize_x;
		int frameSize_y;
	};

private:
	//とりあえずの初期化データ
	const LockOnCircleUIInitData m_defaultLockOnCircle =
	{
		"Assets/spriteData//LockOnCircle/LockOnCircle.DDS",
		400,
		400
	};

private:
	std::shared_ptr<UICanvas> m_canvas;
	std::shared_ptr<UIImage> m_circle; //ロックオン円

	//外部要請用描画フラグ
	bool m_requestVisible = false;
public:
	//コンストラクタ
	LockOnCircle();
	//デストラクタ
	~LockOnCircle() = default;
	//スタート関数
	bool Start() override;
	//アップデート関数
	void Update() override;
	//レンダー関数
	void Render(RenderContext& rc) override;

	//描画開始
	inline void StartLockOnCircleDraw(Enemy* toLockOn)
	{
		m_requestVisible = true;

		m_lockOnCurrent = toLockOn;
	}

	//描画終了
	inline void EndLockOnCircleDraw()
	{
		m_requestVisible = false;

		m_lockOnCurrent = nullptr;
	}

	//ロックオン対象を設定
	inline void SetLockOnCurrent(Enemy* lockOnCurrent) { m_lockOnCurrent = lockOnCurrent; }

	//ロックオンスプライトをリセット
	inline void ResetLockOnReset()
	{
		m_lockOnCurrent = nullptr;
	}

	//初期化
	void Init(const LockOnCircleUIInitData* initData = nullptr);
private:
	//ロックオン対象
	Enemy* m_lockOnCurrent = nullptr;
	//ロックオンスプライトの位置更新
	void LockOnSpritePositionUpdate();
	//ロックオンスプライトのスケール更新
	void LockOnSpriteScaleUpdate();
	//ロックオンスプライトの位置が画面内に存在するかどうかを判定
	bool IsLockOnCicleInScreen();
	//線形補間関数
	const float Leap(const float a, const float b, const float t)
	{
		return (1.0f - t) * a + t * b;
	}
};

