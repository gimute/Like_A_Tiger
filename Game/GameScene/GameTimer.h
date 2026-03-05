#pragma once

class TimerUi;

class GameTimer
{
	//インスタンス
	static GameTimer* m_instance;
	//コンストラクタ
	GameTimer() = default;
	//コピー禁止
	GameTimer(const GameTimer&) = delete;
	//代入禁止にする
	GameTimer& operator=(const GameTimer) = delete;
public:
	//デストラクタ
	~GameTimer() = default;
	//インスタンス取得
	static GameTimer* GetInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new GameTimer();
		}
		return m_instance;
	}

	void InitGameTimer();

	void TimerStart(float time);

	void TimerUpdate();

	inline bool GetEndTimer()
	{
		if (m_timer <= 0.0f)
		{
			return true;
		}

		return false;
	}

	void ResetTimer();

private:
	//タイマーUI
	TimerUi* m_timerUi = nullptr;
	//内部タイマー
	float m_timer = 0.0f;
};

