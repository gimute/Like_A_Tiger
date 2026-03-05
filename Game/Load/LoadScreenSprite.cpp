#include "stdafx.h"
#include "LoadScreenSprite.h"

namespace
{
	//ロード背景サイズ
	static const float LOADBACKSIDE_SPRITE_W_SIZE = 1600.0f;
	static const float LOADBACKSIDE_SPRITE_H_SIZE = 900.0f;
	//ロード回るやつサイズ
	static const float LOADTRUN_SPRITE_W_SIZE = 150.0f;
	static const float LOADTRUN_SPRITE_H_SIZE = 150.0f;
	static Vector3 LOADTRUN_SPRITE_POSITION = Vector3(700.0f, -350.0f, 0.0f);
	//通常ロードフェードイージング数値
	static const float ORDINARYLOAD_EASING_MAX = 1.0f;
	static const float ORDINARYLOAD_EASING_MIN = 0.0f;
}

LoadScreenSprite::LoadScreenSprite()
{
	m_canvas = std::make_shared<UICanvas>();

	//現在は黒画面のみ
	m_loadScreenSprite = m_canvas->CreateUI<UIImage>();

	//ロード中に回るやつ
	m_loadScreenTrunSprite = m_canvas->CreateUI<UIImage>();
}

void LoadScreenSprite::FadeOutExecuting(float fadeSpeed)
{
	//待機状態以外だったら返す
	if (m_screenProcessState != LoadScreenProcees::en_ScreenStandby)
	{
		return;
	}

	//フェードスピード設定
	m_fadeEasingSpeed = fadeSpeed;
	//フェード割合初期化
	m_fadeEasingRatio = 0.0f;

	//フェードのイージングのMAX、MIN設定
	m_fadeEasingMax = ORDINARYLOAD_EASING_MIN;
	m_fadeEasingMin = ORDINARYLOAD_EASING_MAX;

	//透過率を0.0fに
	m_canvas->SetTransmittance(0.0f);

	m_screenProcessState = LoadScreenProcees::en_fadeOutExecuting;
}

void LoadScreenSprite::FadeInExecuting()
{
	//フェードアウト待機以外だったら返す
	if (m_screenProcessState != LoadScreenProcees::en_fadeOutStandby)
	{
		return;
	}

	//フェード割合初期化
	m_fadeEasingRatio = 0.0f;

	//フェードのイージングの、MAX、MIN設定
	m_fadeEasingMax = ORDINARYLOAD_EASING_MAX;
	m_fadeEasingMin = ORDINARYLOAD_EASING_MIN;

	//透過率を1.0fに
	m_canvas->SetTransmittance(1.0f);

	m_screenProcessState = LoadScreenProcees::en_fadeInExecuting;
}

bool LoadScreenSprite::Start()
{
	//スプライト初期化
	m_loadScreenSprite->Init(
		"Assets/spriteData/LoadScreen/LoadScreen_Texture.DDS",
		LOADBACKSIDE_SPRITE_W_SIZE,
		LOADBACKSIDE_SPRITE_H_SIZE
	);

	m_loadScreenTrunSprite->Init(
		"Assets/spriteData/Title/Title_Circle.DDS",
		LOADTRUN_SPRITE_W_SIZE,
		LOADTRUN_SPRITE_H_SIZE
	);

	m_loadScreenTrunSprite->m_transform.m_localPosition = LOADTRUN_SPRITE_POSITION;
	
	//ピボット設定
	m_loadScreenSprite->SetPivot(0.5f,0.5f);
	//透過させる
	m_canvas->SetTransmittance(0.0f);
	
	return true;
}

void LoadScreenSprite::Update()
{
	switch (m_screenProcessState)
	{
		case en_ScreenStandby:
		{

			//フェードアウト実行まで待機中
			m_isFadeInEnd = false;

			break;
		}
		case en_fadeOutExecuting:
		{
			//フェードアウト実行
			float fadeOutEasingRatio = EasingCalc();

			m_canvas->SetTransmittance(fadeOutEasingRatio);

			break;
		}
		case en_fadeOutStandby:
		{

			//フェードイン実行まで待機中
			m_isFadeOutEnd = false;

			LoadScreenTrunUpdate();

			break;
		}
		case en_fadeInExecuting:
		{

			//フェードイン実行
			//フェードアウト実行
			float fadeInEasingRatio = EasingCalc();

			m_canvas->SetTransmittance(fadeInEasingRatio);

			break;
		}
		case en_fadeInCompletion:
		{
			//フェードイン完了
			//完了フラグをtrueに
			m_isFadeInEnd = true;
			//待機に戻す
			m_screenProcessState = LoadScreenProcees::en_ScreenStandby;
			break;
		}
	}

	//キャンバス更新
	m_canvas->Update();
}

float LoadScreenSprite::EasingCalc()
{
	//割合計算
	m_fadeEasingRatio += g_gameTime->GetFrameDeltaTime() * m_fadeEasingSpeed;
	
	//割合が1.0fを超えたら
	if (m_fadeEasingRatio > 1.0f)
	{
		m_fadeEasingRatio = 1.0f;

		if (m_screenProcessState == LoadScreenProcees::en_fadeOutExecuting)
		{
			m_screenProcessState = LoadScreenProcees::en_fadeOutStandby;

			m_isFadeOutEnd = true;
		}
		else if (m_screenProcessState == LoadScreenProcees::en_fadeInExecuting)
		{
			m_screenProcessState = LoadScreenProcees::en_fadeInCompletion;
		}
	}

	return Leap(m_fadeEasingMax, m_fadeEasingMin, m_fadeEasingRatio);
}

void LoadScreenSprite::LoadScreenTrunUpdate()
{
	m_loadScreenTrunSprite->m_transform.m_localRotation.AddRotationDegZ(10.0f);
}

void LoadScreenSprite::Render(RenderContext& rc)
{
	m_canvas->Render(rc);

	if (m_screenProcessState == LoadScreenProcees::en_fadeOutStandby)
	{
		m_loadScreenTrunSprite->Render(rc);
	}
}