#pragma once

class TimerUi: public IGameObject
{
private:
	std::shared_ptr<UICanvas> m_canvas;
	std::shared_ptr<UIText> m_timerText;

	//外部要請用描画フラグ
	bool m_requestVisible = false;
public:
	//コンストラクタ
	TimerUi();
	//デストラクタ
	~TimerUi() = default;
	//スタート関数
	bool Start() override;
	//アップデート関数
	void Update() override;
	//レンダー関数
	void Render(RenderContext& rc) override;

	void SetDrawTime(float time);

	inline void SetVisible(bool visible)
	{
		m_requestVisible = visible;
	}
};

