#include "stdafx.h"
#include "EnemyIntroduction.h"

namespace {
	constexpr const char* BLUE_CIRCLE_PATH = "Assets/spriteData/EnemyIntroduction/blueCircle.DDS";
	constexpr const char* TEXT_CITY_PUNK_PATH = "Assets/spriteData/EnemyIntroduction/text_CiryPunk.DDS";
}

EnemyIntroduction::EnemyIntroduction()
{
}

EnemyIntroduction::~EnemyIntroduction()
{
}

bool EnemyIntroduction::Start()
{
	m_canvas = std::make_shared<UICanvas>();
	m_canvas->m_transform.m_localPosition = Vector3(0.0f, 0.0f, 0.0f);

	m_circleUI = m_canvas->CreateUI<UIImage>();
	m_circleUI->Init(BLUE_CIRCLE_PATH, SCREEN_WIDHT, SCREEN_HEIGHT);
	m_circleUI->m_transform.m_localPosition = Vector3(0.0f, 0.0f, 0.0f);
	m_circleUI->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	/** 拡縮のアニメーション */
	{
		std::vector<float> timeList = { 1.0f };
		std::vector<Vector2> scaleList = { Vector2(2.0f,2.0f),Vector2(1.0f,1.0f) };
		m_scaleCircleUIAnimation = std::make_unique<ScaleUIAnimation>(m_circleUI, false, EasingType::EaseIn, timeList, scaleList);
	}
	/** 色のアニメーション */
	{
		std::vector<float> timeList = { 1.0f,2.0f };
		std::vector<Vector4> colorList = { Vector4(1.0f,1.0f,1.0f,0.0f),Vector4(1.0f,1.0f,1.0f,0.5f),Vector4(1.0f,1.0f,1.0f,0.0f) };
		m_colorCircleUIAnimation = std::make_unique<ColorUIAnimation>(m_circleUI, false, EasingType::EaseIn, timeList, colorList);
	}


	m_textCityPunkUI = m_canvas->CreateUI<UIImage>();
	m_textCityPunkUI->Init(TEXT_CITY_PUNK_PATH, 1024.0f, 576.0f);
	m_textCityPunkUI->m_transform.m_localPosition = Vector3(0.0f, 0.0f, 0.0f);
	m_textCityPunkUI->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	/** 拡縮のアニメーション */
	{
		std::vector<float> timeList = { 1.0f };
		std::vector<Vector2> scaleList = { Vector2(2.0f,2.0f),Vector2(1.0f,1.0f) };
		m_scaleCityPunkUIAnimation = std::make_unique<ScaleUIAnimation>(m_textCityPunkUI, false, EasingType::EaseIn, timeList, scaleList);
	}
	/** 色のアニメーション */
	{
		std::vector<float> timeList = { 1.0f,2.0f };
		std::vector<Vector4> colorList = { Vector4(1.0f,1.0f,1.0f,0.0f),Vector4(1.0f,1.0f,1.0f,1.0f),Vector4(1.0f,1.0f,1.0f,0.0f) };
		m_colorCityPunkUIAnimation = std::make_unique<ColorUIAnimation>(m_textCityPunkUI, false, EasingType::EaseIn, timeList, colorList);
	}

	return true;
}

void EnemyIntroduction::Update()
{
	m_canvas->Update();

	if (m_colorCircleUIAnimation) {
		m_colorCircleUIAnimation->Update();
	}
	if (m_scaleCircleUIAnimation) {
		m_scaleCircleUIAnimation->Update();
	}

	if (m_colorCityPunkUIAnimation) {
		m_colorCityPunkUIAnimation->Update();
	}
	if (m_scaleCityPunkUIAnimation) {
		m_scaleCityPunkUIAnimation->Update();
	}
}

void EnemyIntroduction::Render(RenderContext& rc)
{
	m_canvas->Render(rc);
}
