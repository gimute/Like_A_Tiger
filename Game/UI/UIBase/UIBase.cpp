#include "stdafx.h"
#include "UIBase.h"

// ==============================================
// Canvas
// ==============================================

UICanvas::UICanvas()
{
}


UICanvas::~UICanvas()
{

}


bool UICanvas::Start()
{
	return true;
}


void UICanvas::Update()
{
	m_transform.UpdateTransform();

	for (auto ui : m_uiList) {
		ui->Update();
	}
}


void UICanvas::Render(RenderContext& rc)
{
	for (auto ui : m_uiList) {
		ui->Render(rc);
	}
}

/***************************************/

// ==============================================
// 画像を使うUI
// ==============================================

UIImage::UIImage()
{
}


UIImage::~UIImage()
{
}

void UIImage::Init(const char* filePath, const float w, const float h, AlphaBlendMode alphaBlendMode)
{
	m_spriteRender.Init(filePath, w, h, alphaBlendMode);
	isDraw = true;
}

bool UIImage::Start()
{
	return true;
}


void UIImage::Update()
{
	m_transform.UpdateTransform();
	m_spriteRender.SetTransform(m_transform);
	m_spriteRender.Update();
}

void UIImage::SetPivot(float x, float y)
{
	m_spriteRender.SetPivot(x, y);
}

void UIImage::SetPivot(const Vector2& pivot)
{
	m_spriteRender.SetPivot(pivot);
}

void UIImage::Render(RenderContext& rc)
{
	if (isDraw)
	{
		m_spriteRender.Draw(rc);
	}
}

/************************************/

UIGauge::UIGauge()
{
}


UIGauge::~UIGauge()
{
}

void UIGauge::Init(const char* filePath, const float w, const float h, AlphaBlendMode alphaBlendMode)
{
	m_spriteRender.Init(filePath, w, h, alphaBlendMode);
	isDraw = true;
}

bool UIGauge::Start()
{
	return true;
}

void UIGauge::SetPivot(float x, float y)
{
	m_spriteRender.SetPivot(x, y);
}

void UIGauge::SetPivot(const Vector2& pivot)
{
	m_spriteRender.SetPivot(pivot);
}

const float UIGauge::GetMaxValue() const
{
	return m_maxValue;
}

const float UIGauge::GetValue() const
{
	return m_Value;
}

void UIGauge::Update()
{
	m_transform.UpdateTransform();
	m_spriteRender.SetPosition(m_transform.m_position);
	m_spriteRender.SetRotation(m_transform.m_rotation);
	float scaleX;
	scaleX = m_Value / m_maxValue;
	m_spriteRender.SetScale(Vector3(scaleX, m_transform.m_scale.y, m_transform.m_scale.z));
	m_spriteRender.Update();
}


void UIGauge::Render(RenderContext& rc)
{
	if (isDraw)
	{
		m_spriteRender.Draw(rc);
	}
}

/************************************/

bool UIText::Start()
{
	isDraw = true;

	return false;
}

void UIText::Update()
{
	m_transform.UpdateTransform();
	m_fontRender.SetPosition(m_transform.m_position);
	m_fontRender.SetScale(m_transform.m_scale.x);
}

void UIText::SetText(const wchar_t* text)
{
	m_fontRender.SetText(text);
}

void UIText::SetColor(float r, float g, float b, float a)
{
	m_fontRender.SetColor(r, g, b, a);
}

void UIText::SetColor(const Vector4& color)
{
	m_fontRender.SetColor(color);
}

void UIText::SetPivot(float x, float y)
{
	m_fontRender.SetPivot(x, y);
}

void UIText::SetPivot(const Vector2& pivot)
{
	m_fontRender.SetPivot(pivot);
}

void UIText::Render(RenderContext& rc)
{
	if (isDraw)
	{
		m_fontRender.Draw(rc);
	}
}


