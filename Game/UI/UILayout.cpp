#include "stdafx.h"
#include "UILayout.h"


void UILayout::Update()
{
	int num = 0;

	for (auto ui : m_uiList)
	{
		ui->m_transform.m_localPosition = m_offsetPosition * num;

		num++;
	}
}