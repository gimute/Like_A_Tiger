#pragma once

class GameOver : public IGameObject
{
private:
	enum EnNextType
	{
		enNextType_Retry,
		enNextType_Title,
	};

public:
    struct CursolPositionYData {
        float positionY;
    };

private:
	std::shared_ptr<UICanvas> m_canvas;
	std::shared_ptr<UIImage> m_cursolUI;

	std::unique_ptr<PositionUIAnimation> m_cursolPositionAnimation = nullptr;
	std::unique_ptr<ColorUIAnimation> m_cursolColorAnimation = nullptr;

	int m_currentCursolIndex = 0;

public:
	GameOver();
	~GameOver();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;


	bool IsRetry() const { return m_currentCursolIndex == enNextType_Retry; }
	bool IsTitle() const { return m_currentCursolIndex == enNextType_Title; }
};

