#include "stdafx.h"
#include "GameOver.h"


namespace {
	constexpr const char* GAMEOVER_BASE_PATH = "Assets/spriteData/GameOver/gameOverBase.DDS";
	constexpr const char* GAMEOVER_CURSOL_PATH = "Assets/spriteData/GameOver/gameOverCursol.DDS";
	constexpr const char* GAMEOVER_TEXT_BACKTITLE_PATH = "Assets/spriteData/GameOver/gameOverTextBackTitle.DDS";
	constexpr const char* GAMEOVER_TEXT_RETRY_PATH = "Assets/spriteData/GameOver/gameOverTextRetry.DDS";

	static const Vector2 TEXT_RETRY_POSITION = Vector2(0.0f, 50.0f);
	static const Vector2 TEXT_BACKTITLE_POSITION = Vector2(0.0f, -50.0f);

	constexpr float WAIT_TIME = 0.3f;
	constexpr float ANIMATION_TIME = 0.5f;
}

const GameOver::CursolPositionYData GameOverCursolPositionYData[] = {
	{TEXT_RETRY_POSITION.y},
	{TEXT_BACKTITLE_POSITION.y},
};


GameOver::GameOver()
{
}

GameOver::~GameOver()
{
}

bool GameOver::Start()
{
	m_canvas = std::make_shared<UICanvas>();
	m_canvas->m_transform.m_localPosition = Vector3(0.0f, 0.0f, 0.0f);

	auto gameOverUI = m_canvas->CreateUI<UIImage>();
	gameOverUI->Init(GAMEOVER_BASE_PATH, SCREEN_WIDHT, SCREEN_HEIGHT);
	
	m_cursolUI = m_canvas->CreateUI<UIImage>();
	m_cursolUI->Init(GAMEOVER_CURSOL_PATH, 800.0f, 50.0f);
	m_cursolUI->m_transform.m_localPosition = Vector3(0.0f, 50.0f, 0.0f);
	// カーソルの色イージング
	{
		std::vector<float> timeList = { 0.8f,0.8f };
		std::vector<Vector4> colorList = { Vector4::White,Vector4(1.0f,1.0f,1.0f,0.3f),Vector4::White };
		m_cursolColorAnimation = std::make_unique<ColorUIAnimation>(m_cursolUI, true, EasingType::EaseIn, timeList, colorList);
		m_cursolColorAnimation->Play();
	}

	auto textBackTitleUI = m_canvas->CreateUI<UIImage>();
	textBackTitleUI->Init(GAMEOVER_TEXT_BACKTITLE_PATH, 400.0f, 50.0f);
	textBackTitleUI->m_transform.m_localPosition = Vector3(TEXT_BACKTITLE_POSITION.x, TEXT_BACKTITLE_POSITION.y, 0.0f);
	textBackTitleUI->m_transform.m_localScale = Vector3(0.5f, 0.5f, 1.0f);

	auto textRetryUI = m_canvas->CreateUI<UIImage>();
	textRetryUI->Init(GAMEOVER_TEXT_RETRY_PATH, 200.0f, 50.0f);
	textRetryUI->m_transform.m_localPosition = Vector3(TEXT_RETRY_POSITION.x, TEXT_RETRY_POSITION.y, 0.0f);
	textRetryUI->m_transform.m_localScale = Vector3(0.5f, 0.5f, 1.0f);

	return true;
}

void GameOver::Update()
{
	m_cursolColorAnimation->Update();

	if (g_pad[0]->IsTrigger(enButtonUp))
	{
		m_currentCursolIndex--;
		if (m_currentCursolIndex < 0) {
			m_currentCursolIndex = 0;
		}
	}
	
	if (g_pad[0]->IsTrigger(enButtonDown))
	{
		m_currentCursolIndex++;
		if (m_currentCursolIndex > 1) {
			m_currentCursolIndex = 1;
		}
	}
	if (m_currentCursolIndex >= 0
		&& m_currentCursolIndex <= 1) {
		const auto& pos = GameOverCursolPositionYData[m_currentCursolIndex];
		m_cursolUI->m_transform.m_localPosition = Vector3(0.0f, pos.positionY, 0.0f);
	}
	

	m_canvas->Update();
}

void GameOver::Render(RenderContext& rc)
{
	m_canvas->Render(rc);
}
