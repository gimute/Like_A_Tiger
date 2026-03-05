#include "stdafx.h"
#include "TimerUi.h"

namespace TimeUiConstant
{
	const Vector3 TIMER_UI_POS = Vector3(-600.0f, -465, 0.0f);
}

TimerUi::TimerUi()
{
	m_canvas = std::make_shared<UICanvas>();

	m_timerText = m_canvas->CreateUI<UIText>();
}

bool TimerUi::Start()
{
	m_canvas->m_transform.m_localPosition = TimeUiConstant::TIMER_UI_POS;

	wchar_t buffer[256];

	swprintf_s(
		buffer,
		256,
		L"%01d:%02d",
		float(0),
		float(0)
	);

	m_timerText->SetText(buffer);

	return true;
}

void TimerUi::Update()
{
	m_canvas->Update();
}

void TimerUi::Render(RenderContext& rc)
{
	if (m_requestVisible)
	{
		m_canvas->Render(rc);
	}
}

void TimerUi::SetDrawTime(float time)
{
	wchar_t buffer[256];

	int totalSeconds = static_cast<int>(std::ceil(time));

	if (totalSeconds < 0)
	{
		totalSeconds = 0;
	}

	int minutes = totalSeconds / 60;
	int seconds = totalSeconds % 60;

	swprintf_s(
		buffer,
		256,
		L"%01d:%02d",
		minutes,
		seconds
	);

	m_timerText->SetText(buffer);
}