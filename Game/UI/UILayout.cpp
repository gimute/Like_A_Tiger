#include "stdafx.h"
#include "UILayout.h"


void UILayout::Update()
{
	int offsetCount = 0;

	//改行条件が0以下なら
	if (m_newLineNum <= 0)
	{
		//改行機能なしの配置処理
		for (auto ui : m_uiList)
		{
			ui->m_transform.m_localPosition = m_offsetPosition * offsetCount;

			offsetCount++;
		}
	}
	else
	{
		//改行機能付きの配置処理
		int newLineCount = 0;

		for (auto ui : m_uiList)
		{
			//オフセットカウントが改行する数まで進んだら
			if (offsetCount% m_newLineNum == 0)
			{
				//オフセットカウントリセット
				offsetCount = 0;
				//改行カウント
				newLineCount++;
			}

			ui->m_transform.m_localPosition = m_offsetPosition * offsetCount + m_newLineOffsetPos * newLineCount;

			offsetCount++;
		}
	}
}