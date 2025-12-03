#include "stdafx.h"
#include "UIAnimation.h"

void UIAnimationBase::UpdateCore()
{
	//時間経過
	m_elapsedTime += g_gameTime->GetFrameDeltaTime();

	//再生中のアニメーションの全体時間
	float targetTime = m_animationTimeList[m_targetIndex];

	//経過時間の割合
	const float elapsedPercent = m_elapsedTime / targetTime;

	//経過時間の割合を元に変更を与える
	UpdateValue(elapsedPercent);

	//再生しきったら次のアニメーションへ
	if (m_elapsedTime >= targetTime)
	{
		m_targetIndex++;
		m_elapsedTime = 0.0f;

		
		if (m_targetIndex == m_animationTimeList.size())
		{
			//全て再生完了
			m_isCompleted = true;
			m_targetIndex = 0;
		}
	}
}

bool UIAnimationBase::CanUpdate()
{
	//再生中か？
	if (!m_isPlay)
	{
		return false;
	}

	//再生終了しているか？
	if (m_isCompleted)
	{
		//ループするか？
		if (m_isLoop)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return true;
}

/*
* 座標アニメーション
*/

void PositionUIAnimation::UpdateValue(float elapsedPercent)
{
	//座標を線形補完
	Vector3 startPosition = m_targetPositionList[m_targetIndex];
	Vector3 endPosition = m_targetPositionList[m_targetIndex + 1];

	Vector3 nowPosition = nsK2EngineLow::Math::Lerp(elapsedPercent, startPosition, endPosition);

	m_ui->m_transform.m_localPosition = nowPosition;
}

void PositionUIAnimation::Update()
{
	if (!CanUpdate())
	{
		return;
	}

	UpdateCore();
}

/*
* 相対座標アニメーション
*/
void OffsetPositionUIAnimation::UpdateValue(float elapsedPercent)
{
	//ずらす座標を線形補完
	Vector3 startPosition = m_targetOffsetPositionList[m_targetIndex];
	Vector3 endPosition = m_targetOffsetPositionList[m_targetIndex + 1];

	Vector3 offsetPosition = nsK2EngineLow::Math::Lerp(elapsedPercent, startPosition, endPosition);

	m_ui->m_transform.m_localPosition = m_referencePosition + offsetPosition;
}

void OffsetPositionUIAnimation::Update()
{
	if (!CanUpdate())
	{
		return;
	}

	UpdateCore();
}

/*
* 回転アニメーション
*/
void RotarionUIAnimation::UpdateValue(float elapsedPercent)
{
	//スケールを線形補完
	Quaternion startRotation = m_targetRotationList[m_targetIndex];
	Quaternion endRotation = m_targetRotationList[m_targetIndex + 1];

	Quaternion nowRotation = nsK2EngineLow::Math::Lerp(elapsedPercent, startRotation, endRotation);

	m_ui->m_transform.m_localRotation = nowRotation;
}

void RotarionUIAnimation::Update()
{
	if (!CanUpdate())
	{
		return;
	}

	UpdateCore();
}


/*
* スケールアニメーション
*/
void ScaleUIAnimation::UpdateValue(float elapsedPercent)
{
	//スケールを線形補完
	Vector2 startScale = m_targetScaleList[m_targetIndex];
	Vector2 endScale = m_targetScaleList[m_targetIndex + 1];

	Vector2 nowScale = nsK2EngineLow::Math::Lerp(elapsedPercent, startScale, endScale);

	m_ui->m_transform.m_localScale = Vector3(nowScale.x, nowScale.y, 1.0f);
}

void ScaleUIAnimation::Update()
{
	if (!CanUpdate())
	{
		return;
	}

	UpdateCore();
}

/*
* 色アニメーション
*/

void ColorUIAnimation::UpdateValue(float elapsedPercent)
{
	//スケールを線形補完
	Vector4 startColor = m_targetColorList[m_targetIndex];
	Vector4 endColor = m_targetColorList[m_targetIndex + 1];

	Vector4 nowColor = nsK2EngineLow::Math::Lerp(elapsedPercent, startColor, endColor);

	m_ui->SetColor(nowColor);
}

void ColorUIAnimation::Update()
{
	if (!CanUpdate())
	{
		return;
	}

	UpdateCore();
}


