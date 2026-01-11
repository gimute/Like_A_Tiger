#include "stdafx.h"
#include "UIBase.h"

// ==============================================
// Canvas
// ==============================================

UICanvas::UICanvas()
{
	m_isDraw = true;
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
	if (!m_isUpdate)
	{
		return;
	}

	m_transform.UpdateTransform();

	for (auto ui : m_uiList) {
		ui->Update();
	}
}


void UICanvas::Render(RenderContext& rc)
{
	if (!m_isDraw)
	{
		return;
	}

	for (auto ui : m_uiList) {
		ui->Render(rc);
	}
}

void UICanvas::SetColor(Vector4 mulColor)
{
	for (auto ui : m_uiList)
	{
		ui->SetColor(mulColor);
	}
}

void UICanvas::SetColor(float r, float g, float b, float a)
{
	for (auto ui : m_uiList)
	{
		ui->SetColor(Vector4(r, g, b, a));
	}
}

void UICanvas::SetTransmittance(float setTransmittance)
{
	for (auto ui : m_uiList)
	{
		ui->SetTransmittance(setTransmittance);
	}
}

Vector4 UICanvas::GetColor()
{
	//一旦白を返すようにしておく
	return Vector4(1.0f,1.0f,1.0f,1.0f);
}

void UICanvas::SetDrawFlag(bool visible)
{
	m_isDraw = visible;
}

void UICanvas::SetUpdateFlag(bool enable)
{
	m_isUpdate = enable;
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
	m_isDraw = true;
}

bool UIImage::Start()
{
	m_isStart = true;

	return true;
}


void UIImage::Update()
{
	if (!m_isUpdate)
	{
		return;
	}
	m_transform.UpdateTransform();
	m_spriteRender.SetTransform(m_transform);
	m_spriteRender.SetMulColor(m_mulColor);
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

void UIImage::SetColor(Vector4 color)
{
	m_mulColor = color;
}

void UIImage::SetColor(float r, float g, float b, float a)
{
	m_mulColor = Vector4(r, g, b, a);
}

void UIImage::SetTransmittance(float setTransmittance)
{
	m_mulColor.a = setTransmittance;
}

Vector4 UIImage::GetColor()
{
	return m_mulColor;
}

void UIImage::Render(RenderContext& rc)
{
	if (m_isDraw)
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
	m_isDraw = true;
}

bool UIGauge::Start()
{
	m_isStart = true;
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

void UIGauge::SetColor(Vector4 mulColor)
{
	m_mulColor = mulColor;
}

void UIGauge::SetColor(float r, float g, float b, float a)
{
	m_mulColor = Vector4(r, g, b, a);
}

Vector4 UIGauge::GetColor()
{
	return m_mulColor;
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
	if (!m_isUpdate)
	{
		return;
	}

	m_transform.UpdateTransform();
	m_spriteRender.SetPosition(m_transform.m_position);
	m_spriteRender.SetRotation(m_transform.m_rotation);
	m_spriteRender.SetMulColor(m_mulColor);

	//ゲージの伸び縮み用スケール計算
	float scaleX;
	scaleX = m_Value / m_maxValue;

	m_spriteRender.SetScale(Vector3(m_transform.m_scale.x * scaleX, m_transform.m_scale.y, m_transform.m_scale.z));
	m_spriteRender.Update();
}


void UIGauge::Render(RenderContext& rc)
{
	if (m_isDraw)
	{
		m_spriteRender.Draw(rc);
	}
}

/************************************/

bool UIText::Start()
{
	m_isDraw = true;
	m_isStart = true;
	return true;
}

void UIText::Update()
{
	if (!m_isUpdate)
	{
		return;
	}
	m_transform.UpdateTransform();
	m_fontRender.SetPosition(m_transform.m_position);
	m_fontRender.SetScale(m_transform.m_scale.x);
	m_fontRender.SetColor(m_color);
}

void UIText::SetText(const wchar_t* text)
{
	m_fontRender.SetText(text);
}

void UIText::SetColor(Vector4 color)
{
	m_color = color;
}

void UIText::SetColor(float r, float g, float b, float a)
{
	m_color = Vector4(r, g, b, a);
}

Vector4 UIText::GetColor()
{
	return m_color;
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
	if (m_isDraw)
	{
		m_fontRender.Draw(rc);
	}
}


