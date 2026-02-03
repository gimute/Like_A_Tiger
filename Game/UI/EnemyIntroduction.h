#pragma once

class EnemyIntroduction : public IGameObject
{
public:
	EnemyIntroduction();
	~EnemyIntroduction();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	void PlayAnimation() {
		if (m_colorCircleUIAnimation) {
			m_colorCircleUIAnimation->Play();
		}
		if (m_scaleCircleUIAnimation) {
			m_scaleCircleUIAnimation->Play();
		}
		if (m_colorCityPunkUIAnimation) {
			m_colorCityPunkUIAnimation->Play();
		}
		if (m_scaleCityPunkUIAnimation) {
			m_scaleCityPunkUIAnimation->Play();
		}
	}

private:
	std::shared_ptr<UICanvas> m_canvas;
	std::shared_ptr<UIImage> m_circleUI;
	std::shared_ptr<UIImage> m_textCityPunkUI;

	/** 青い円用アニメーション */
	std::shared_ptr<ColorUIAnimation> m_colorCircleUIAnimation;
	std::shared_ptr<ScaleUIAnimation> m_scaleCircleUIAnimation;
	/** テキスト　街の不良用アニメーション */
	std::shared_ptr<ColorUIAnimation> m_colorCityPunkUIAnimation;
	std::shared_ptr<ScaleUIAnimation> m_scaleCityPunkUIAnimation;
};

