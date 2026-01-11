#include "stdafx.h"
#include "Result.h"

Result::Result()
{
	m_canvas = std::make_shared<UICanvas>();

	m_resultUI = m_canvas->CreateUI<UIImage>();
}

Result::~Result()
{

}

bool Result::Start()
{
	return true;
}

void Result::Update()
{
	m_canvas->Update();
}

void Result::Render(RenderContext& rc)
{
	m_canvas->Render(rc);
}

void Result::Init(const ResultInitData* initData)
{
	if (initData == nullptr)
	{
		Init(&defaultResult);
		return;
	}

	m_resultUI->Init(
		initData->resultFilePath,
		initData->frameSize_x,
		initData->frameSize_y
	);

	m_resultUI->SetPivot(0.5f, 0.5f);

	m_canvas->m_transform.m_localPosition = Vector3(0.0f, 0.0f, 0.0f);
}