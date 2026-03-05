#include "stdafx.h"
#include "GameTimer.h"

#include "UI\TimerUi.h"
#include "GameScene\UpdateOrder.h"

//インスタンス初期化
GameTimer* GameTimer::m_instance = nullptr;

void GameTimer::InitGameTimer()
{
	m_timerUi = NewGO<TimerUi>(UpdateOrder::UI, "Ui");
}

void GameTimer::TimerStart(float time)
{
	m_timer = time;

	m_timerUi->SetVisible(true);
}

void GameTimer::TimerUpdate()
{
	if (m_timer <= 0.0)
	{
		return;
	}

	m_timer -= g_gameTime->GetFrameDeltaTime();

	m_timerUi->SetDrawTime(m_timer);
}

void GameTimer::ResetTimer()
{
	DeleteGO(m_timerUi);

	m_timerUi->SetVisible(false);

	m_timer = 0.0f;
}