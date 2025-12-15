#include "stdafx.h"
#include "UIAnimation.h"


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

void PositionUIAnimation::Update()
{
	if (!CanUpdate())
	{
		return;
	}

	m_curve.Update();

	m_ui->m_transform.m_localPosition = m_curve.GetCurrentValue();


	if (!m_curve.IsPlaying())
	{
		//次を再生
		m_targetIndex++;

		//再生しきったか
		if (m_targetIndex == m_animationTimeList.size())
		{
			//全て再生完了
			m_isCompleted = true;
			m_targetIndex = 0;

		}
		
		m_curve.Play(
			m_targetPositionList[m_targetIndex],
			m_targetPositionList[m_targetIndex + 1],
			m_animationTimeList[m_targetIndex],
			m_easignType,
			LoopMode::Once
		);

	}
}

/*
* 相対座標アニメーション
*/

void OffsetPositionUIAnimation::Update()
{
	if (!CanUpdate())
	{
		return;
	}

	m_curve.Update();

	m_ui->m_transform.m_localPosition = m_referencePosition + m_curve.GetCurrentValue();


	if (!m_curve.IsPlaying())
	{
		//次を再生
		m_targetIndex++;

		//再生しきったか
		if (m_targetIndex == m_animationTimeList.size())
		{
			//全て再生完了
			m_isCompleted = true;
			m_targetIndex = 0;

		}

		m_curve.Play(
			m_targetOffsetPositionList[m_targetIndex],
			m_targetOffsetPositionList[m_targetIndex + 1],
			m_animationTimeList[m_targetIndex],
			m_easignType,
			LoopMode::Once
		);

	}
}

/*
* 回転アニメーション
*/

void RotarionUIAnimation::Update()
{
	if (!CanUpdate())
	{
		return;
	}

	m_curve.Update();

	Quaternion tmpRot;
	tmpRot.AddRotationDegZ(m_curve.GetCurrentValue());

	m_ui->m_transform.m_localRotation = tmpRot;

	if (!m_curve.IsPlaying())
	{
		//次を再生
		m_targetIndex++;

		//再生しきったか
		if (m_targetIndex == m_animationTimeList.size())
		{
			//全て再生完了
			m_isCompleted = true;
			m_targetIndex = 0;
		}

		m_curve.Play(
			m_targetZRotationList[m_targetIndex],
			m_targetZRotationList[m_targetIndex + 1],
			m_animationTimeList[m_targetIndex],
			m_easignType,
			LoopMode::Once
		);
	}
}


/*
* スケールアニメーション
*/

void ScaleUIAnimation::Update()
{
	if (!CanUpdate())
	{
		return;
	}

	m_curve.Update();

	Vector2 scale = m_curve.GetCurrentValue();

	m_ui->m_transform.m_localScale = Vector3(scale.x, scale.y, 1.0f);


	if (!m_curve.IsPlaying())
	{
		//次を再生
		m_targetIndex++;

		//再生しきったか
		if (m_targetIndex == m_animationTimeList.size())
		{
			//全て再生完了
			m_isCompleted = true;
			m_targetIndex = 0;

		}

		m_curve.Play(
			m_targetScaleList[m_targetIndex],
			m_targetScaleList[m_targetIndex + 1],
			m_animationTimeList[m_targetIndex],
			m_easignType,
			LoopMode::Once
		);

	}
}

/*
* 色アニメーション
*/


void ColorUIAnimation::Update()
{
	if (!CanUpdate())
	{
		return;
	}

	m_curve.Update();

	m_ui->SetColor(m_curve.GetCurrentValue());

	if (!m_curve.IsPlaying())
	{
		//次を再生
		m_targetIndex++;

		//再生しきったか
		if (m_targetIndex == m_animationTimeList.size())
		{
			//全て再生完了
			m_isCompleted = true;
			m_targetIndex = 0;

		}

		m_curve.Play(
			m_targetColorList[m_targetIndex],
			m_targetColorList[m_targetIndex + 1],
			m_animationTimeList[m_targetIndex],
			m_easignType,
			LoopMode::Once
		);
	}

}


