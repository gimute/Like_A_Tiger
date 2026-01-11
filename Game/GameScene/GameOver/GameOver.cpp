#include "stdafx.h"
#include "GameOver.h"

GameOver::GameOver()
{
	m_canvas = std::make_shared<UICanvas>();

	m_gameOverUI = m_canvas->CreateUI<UIImage>();
}

GameOver::~GameOver()
{

}

bool GameOver::Start()
{
	return true;
}

void GameOver::Update()
{
	m_canvas->Update();
}

void GameOver::Render(RenderContext& rc)
{
	m_canvas->Render(rc);
}

void GameOver::Init(const GameOverInitData* initData)
{
	if (initData == nullptr)
	{
		Init(&defaultGameOver);
		return;
	}

	m_gameOverUI->Init(
		initData->gameOverFilePath,
		initData->frameSize_x,
		initData->frameSize_y
	);

	m_gameOverUI->SetPivot(0.5f, 0.5f);

	m_canvas->m_transform.m_localPosition = Vector3(0.0f, 0.0f, 0.0f);
}