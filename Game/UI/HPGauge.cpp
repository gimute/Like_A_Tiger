#include "stdafx.h"
#include "HPGauge.h"

HPGauge::HPGauge()
{
	m_frame = m_canvas.CreateUI<UIImage>();
	m_delayedHPBar = m_canvas.CreateUI<UIGauge>();
	m_HPBar = m_canvas.CreateUI<UIGauge>();
}

HPGauge::~HPGauge()
{
}

bool HPGauge::Start()
{
	return true;
}

void HPGauge::Update()
{
	DelaydHPBarUpdate();

	m_canvas.Update();
}

void HPGauge::Render(RenderContext& rc)
{
	m_canvas.Render(rc);
}

void HPGauge::Init(const HPGaugeUIInitData* initData)
{
	if (initData == nullptr)
	{
		//設定されていなければ仮画像で初期化
		Init(&defaultHPGauge);
		return;
	}

	//フレームの初期化
	m_frame->Init(initData->frameFilePath, initData->frameSize_x, initData->frameSize_y);
	m_frame->SetPivot(0.0f, 1.0f);

	//ディレイHPバーを初期化
	m_delayedHPBar->Init(initData->delayedHPBarFilePath, initData->hpBarSize_x, initData->hpBarSize_y);
	m_delayedHPBar->SetPivot(0.0f, 1.0f);
	//フレームとバーの位置を合わせる
	m_delayedHPBar->m_transform.m_localPosition = m_delayedHPBar->m_transform.m_localPosition + Vector3((initData->frameSize_x - initData->hpBarSize_x) / 2, -((initData->frameSize_y - initData->hpBarSize_y) / 2), 0.0f);

	//HPバーを初期化
	m_HPBar->Init(initData->hpBarFilePath, initData->hpBarSize_x, initData->hpBarSize_y);
	m_HPBar->SetPivot(0.0f, 1.0f);
	//フレームとバーの位置を合わせる
	m_HPBar->m_transform.m_localPosition = m_HPBar->m_transform.m_localPosition + Vector3((initData->frameSize_x - initData->hpBarSize_x) / 2, -((initData->frameSize_y - initData->hpBarSize_y) / 2), 0.0f);

}

void HPGauge::SetMaxHP(float maxHP)
{
	m_HPBar->SetMaxValue(maxHP);
	m_delayedHPBar->SetMaxValue(maxHP);
}

void HPGauge::SetHP(float nowHP)
{
	m_HPBar->SetValue(nowHP);

	if (m_delayedHPBar->GetValue() < nowHP)
	{
		//ディレイHPバーが現在HPより少ないなら即座に現在HPに合わせる
		m_delayedHPBar->SetValue(nowHP);
	}
	else
	{
		InitializeDelaydHPBar();
	}
}

void HPGauge::DecreaseHP(float val)
{
	SetHP(m_HPBar->GetValue() - val);
}

void HPGauge::IncreaseHP(float val)
{
	SetHP(m_HPBar->GetValue() + val);
}

void HPGauge::SetPosition(Vector3 pos)
{
	m_canvas.m_transform.m_localPosition = pos;
}

void HPGauge::InitializeDelaydHPBar()
{
	//ディレイHPバーのラープをリセット
	m_lerpVal = 0.0f;

	//ディレイHPバーがHPバーに追従するまでのタイマーをセット
	m_delayTimer = 0.3f;
}

void HPGauge::DelaydHPBarUpdate()
{
	//lerpが終わっていたら全飛ばし
	if (m_lerpVal >= 1.0f)
	{
		return;
	}

	if (m_delayTimer < 0)
	{
		//lerpでHPバーに追従
		m_lerpVal += 1.0f * g_gameTime->GetFrameDeltaTime();
		m_delayedHPBar->SetValue((m_HPBar->GetValue() * m_lerpVal) + (m_delayedHPBar->GetValue() * (1 - m_lerpVal)));;
	}
	else
	{
		m_delayTimer -= g_gameTime->GetFrameDeltaTime();
	}
}


