#include "stdafx.h"
#include "LockOnCircle.h"

#include "Actor\Enemy\Enemy.h"

namespace
{
	float MIN_DIST = 2.0f;
	float MAX_DIST = 20.0f;

	float MAX_SCALE = 1.2;
	float MIN_SCALE = 0.6;
}

LockOnCircle::LockOnCircle()
{
	m_canvas = std::make_shared<UICanvas>();

	m_circle = m_canvas->CreateUI<UIImage>();
}

bool LockOnCircle::Start()
{
	return true;
}

void LockOnCircle::Update()
{
	LockOnSpritePositionUpdate();

	LockOnSpriteScaleUpdate();

	m_canvas->Update();
}

void LockOnCircle::Render(RenderContext& rc)
{
	if (m_requestVisible && IsLockOnCicleInScreen())
	{
		m_canvas->Render(rc);
	}
}

void LockOnCircle::Init(const LockOnCircleUIInitData* initData)
{
	if (initData == nullptr)
	{
		//設定されていなければ仮画像で初期化
		Init(&m_defaultLockOnCircle);
		return;
	}

	m_circle->Init(
		initData->frameFilePath,
		initData->frameSize_x,
		initData->frameSize_y
	);
}

void LockOnCircle::LockOnSpritePositionUpdate()
{
	if (!m_requestVisible)
	{
		return;
	}

	//ロックオン対象の位置を取得
	Vector3 currentPos = m_lockOnCurrent->GetPosition();
	//対象の少し上に表示したいのでy座標に少し加算
	currentPos.y += 50.0f;
	//ワールド座標からスクリーン座標へ
	Vector2 spritePos2D = Vector2::Zero;
	g_camera3D->CalcScreenPositionFromWorldPosition(spritePos2D, currentPos);
	//位置設定の為に3vecに
	Vector3 spritePos = Vector3::Zero;
	spritePos.x = spritePos2D.x;
	spritePos.y = spritePos2D.y;

	m_canvas->m_transform.m_localPosition = spritePos;
}

void LockOnCircle::LockOnSpriteScaleUpdate()
{
	if (!m_requestVisible)
	{
		return;
	}

	//ロックオン対象の位置を取得
	Vector3 currentPos = m_lockOnCurrent->GetPosition();
	//対象とカメラの距離を計算
	Vector3 toCurrent = currentPos - g_camera3D->GetPosition();
	float dist = toCurrent.Length();

	float t = (dist - MIN_DIST) / (MAX_DIST - MIN_DIST);
	t = btClamped(t, 0.0f, 1.0f);

	float scale = Leap(MAX_SCALE, MIN_SCALE, t);

	Vector3 setScale = Vector3::Zero;
	setScale.x = scale;
	setScale.y = scale;
 
	m_canvas->m_transform.m_localScale = setScale;
}

bool LockOnCircle::IsLockOnCicleInScreen()
{
	if (!m_lockOnCurrent)
	{
		return false;
	}
	//ここの実装良くわからんので藤原君に聞いてみる
	return true;
}


