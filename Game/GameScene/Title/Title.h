#pragma once

class Title : public IGameObject {
private:
	std::shared_ptr<UICanvas> m_canvas;
	std::shared_ptr<UIImage> m_iconButtonUI = nullptr;

	std::unique_ptr<ScaleUIAnimation> m_circleAnimation = nullptr;
	std::unique_ptr<ScaleUIAnimation> m_iconAAnimation = nullptr;
	std::unique_ptr<ScaleUIAnimation> m_logoAnimation = nullptr;

	std::unique_ptr<ColorUIAnimation> m_circleColorAnimation = nullptr;
	std::unique_ptr<ColorUIAnimation> m_iconColorAnimation = nullptr;

	std::unique_ptr<ColorUIAnimation> m_buttonAnimation = nullptr;

	// タイトル演出したか
	bool m_isTitlePlay = false;


public:
	Title();
	~Title();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
};


