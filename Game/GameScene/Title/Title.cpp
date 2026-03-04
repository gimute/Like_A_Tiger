#include "stdafx.h"
#include "Title.h"


namespace
{
	constexpr const char* BACKGROUND_PATH = "Assets/spriteData/Title/Title_backGround.DDS";
	constexpr const char* CIRCLE_PATH = "Assets/spriteData/Title/Title_Circle.DDS";
	constexpr const char* ICON_A_PATH = "Assets/spriteData/Title/Title_A_Icon.DDS";
	constexpr const char* LOGO_PATH = "Assets/spriteData/Title/Title_LikeTiger.DDS";
	constexpr const char* ICON_BUTTON_PATH = "Assets/spriteData/Title/Title_PressBButton.DDS";
	constexpr const char* ICON_BUTTON_BASE_PATH = "Assets/spriteData/Title/Title_PressAnyButton_Base.DDS";


	constexpr float WAIT_TIME = 0.2f;
	constexpr float ANIMATION_TIME = 0.3f;
}



Title::Title(){
}

Title::~Title() {

}

bool Title::Start() {

	m_canvas = std::make_shared<UICanvas>();
	m_canvas->m_transform.m_localPosition = Vector3(0.0f, 40.0f, 0.0f);

	auto titleUI = m_canvas->CreateUI<UIImage>();
	titleUI->Init(BACKGROUND_PATH, SCREEN_WIDHT, SCREEN_HEIGHT);
	titleUI->SetPivot(0.5f, 0.5f);

	auto circleUI = m_canvas->CreateUI<UIImage>();
	circleUI->Init(CIRCLE_PATH, 500.0f, 500.0f);
	circleUI->m_transform.m_localPosition = Vector3(150.0f, 30.0f, 0.0f);
	circleUI->m_transform.m_localRotation.SetRotationDegZ(-30.0f);
	{
		std::vector<float> timeList = { WAIT_TIME*2.0f, ANIMATION_TIME };
		std::vector<Vector2> scaleList = { Vector2(3.0f,3.0f), Vector2(3.0f,3.0f),Vector2(1.0f,1.0f) };
		m_circleAnimation = std::make_unique<ScaleUIAnimation>(circleUI, false, EasingType::EaseIn, timeList, scaleList);
		m_circleAnimation->Play();
	}
	{
		std::vector<float> timeList = { WAIT_TIME*2.0f };
		std::vector<Vector4> colorList = { Vector4(1.0f,1.0f,1.0f, 0.0f), Vector4::White };
		m_circleColorAnimation = std::make_unique<ColorUIAnimation>(circleUI, false, EasingType::EaseIn, timeList, colorList);
		m_circleColorAnimation->Play();
	}

	auto iconAUI = m_canvas->CreateUI<UIImage>();
	iconAUI->Init(ICON_A_PATH, 300.0f, 300.0f);
	iconAUI->m_transform.m_localPosition = Vector3(150.0f, 100.0f, 0.0f);
	// Aアイコンのアニメーション
	{
		std::vector<float> timeList = { WAIT_TIME, ANIMATION_TIME };
		std::vector<Vector2> scaleList = { Vector2(3.0f,3.0f), Vector2(3.0f,3.0f),Vector2(1.0f,1.0f) };
		m_iconAAnimation = std::make_unique<ScaleUIAnimation>(iconAUI, false, EasingType::EaseIn, timeList, scaleList);
		m_iconAAnimation->Play();
	}
	{
		std::vector<float> timeList = { WAIT_TIME };
		std::vector<Vector4> colorList = { Vector4(1.0f,1.0f,1.0f, 0.0f), Vector4::White };
		m_iconColorAnimation = std::make_unique<ColorUIAnimation>(iconAUI, false, EasingType::EaseIn, timeList, colorList);
		m_iconColorAnimation->Play();
	}

	auto logoUI = m_canvas->CreateUI<UIImage>();
	logoUI->Init(LOGO_PATH, 700.0f, 450.0f);
	logoUI->m_transform.m_localPosition = Vector3(-150.0f, 0.0f, 0.0f);
	// ロゴのアニメーション
	{
		std::vector<float> timeList = { ANIMATION_TIME };
		std::vector<Vector2> scaleList = { Vector2(3.0f,3.0f),Vector2(1.0f,1.0f)};
		m_logoAnimation = std::make_unique<ScaleUIAnimation>(logoUI, false, EasingType::EaseIn, timeList, scaleList);
		m_logoAnimation->Play();
	}

	m_iconButtonUI = m_canvas->CreateUI<UIImage>();
	m_iconButtonUI->Init(ICON_BUTTON_BASE_PATH, 800.0f, 60.0f);
	m_iconButtonUI->m_transform.m_localPosition = Vector3(0.0f, -350.0f, 0.0f);
	m_iconButtonUI->SetDrawFlag(false);
	// ボタンのアニメーション
	{
		std::vector<float> timeList = { 0.8f, 0.8f };
		std::vector<Vector4> colorList = { Vector4::White, Vector4(1.0f, 1.0f, 1.0f, 0.3f), Vector4::White };
		m_buttonAnimation = std::make_unique<ColorUIAnimation>(m_iconButtonUI, true, EasingType::EaseInOut, timeList, colorList);
		m_buttonAnimation->Play();
	}

	auto buttonUI = m_canvas->CreateUI<UIImage>();
	buttonUI->Init(ICON_BUTTON_PATH, 350.0f, 30.0f);
	buttonUI->m_transform.m_localPosition = Vector3(0.0f, -350.0f, 0.0f);
	
	return true;
}

void Title::Update() {
	m_circleAnimation->Update();
	m_iconAAnimation->Update();
	m_logoAnimation->Update();

	m_circleColorAnimation->Update();
	m_iconColorAnimation->Update();

	m_buttonAnimation->Update();

	if (!m_isTitlePlay) {
		if (m_circleAnimation->IsCompleted()) {
			m_iconButtonUI->SetDrawFlag(true);
			m_isTitlePlay = true;
		}
	}

	m_canvas->Update();
}

void Title::Render(RenderContext& rc) {
	m_canvas->Render(rc);
}