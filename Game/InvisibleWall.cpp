#include "stdafx.h"
#include "InvisibleWall.h"

///////////////////////////////////////////////////////////////////////////////

InvisibleWallShape* InvisibleWallShape::m_instance = nullptr;

InvisibleWallShape::InvisibleWallShape()
{
	ModelInitData initData;

	initData.m_tkmFilePath = "Assets/modelData/Collision/Circle.tkm";
	m_shapeModels[Circle].Init(initData);

}

///////////////////////////////////////////////////////////////////////////////

BoxInvisibleWall::BoxInvisibleWall()
{
}

BoxInvisibleWall::~BoxInvisibleWall()
{
}

bool BoxInvisibleWall::Start()
{
	return true;
}

void BoxInvisibleWall::Update()
{
}

void BoxInvisibleWall::Create(Vector3 pos, Vector3 size)
{
}

void BoxInvisibleWall::Delete()
{

}

///////////////////////////////////////////////////////////////////////////////


CircleInvisibleWall::CircleInvisibleWall()
{
	m_wallEffect.Init(3);
	m_lineEffect.Init(4);
}

CircleInvisibleWall::~CircleInvisibleWall()
{
}

bool CircleInvisibleWall::Start()
{

	return true;
}

void CircleInvisibleWall::Update()
{
	if (m_active)
	{


		m_wallEffect.Update();
		m_lineEffect.Update();

		if (!m_lineEffect.IsPlay())
		{
			m_lineEffect.Play();
			m_lineEffect.Update();
		}

		if (!m_wallEffect.IsPlay())
		{
			m_wallEffect.Play();
			m_wallEffect.Update();
		}
	}
	else
	{

		m_wallEffect.Stop();
	}
}

void CircleInvisibleWall::Create(Vector3 pos, float radius)
{
	if (m_active)
	{
		return;
	}

	//透明壁の指定されたサイズにするための拡大率
	float scale = radius / CIRCLE_MODEL_RADIUS;

	Model* circleModel = InvisibleWallShape::GetInstance().GetShapeModel(InvisibleWallShape::Circle);

	//カメラと干渉しないよう縦のスケールは等倍
	circleModel->UpdateWorldMatrix(pos, Quaternion::Identity, Vector3(scale, 1.0f, scale));

	//モデルから透明壁を生成
	m_collision.CreateFromModel(*circleModel, circleModel->GetWorldMatrix());

	m_wallEffect.SetPosition(pos);
	m_wallEffect.SetScale(Vector3(1.0f,1.0f, 1.0f));
	m_wallEffect.Play();

	m_lineEffect.SetPosition(pos);
	m_lineEffect.SetScale(Vector3(1.0, 1.0f, 1.0));
	m_lineEffect.Play();

	m_active = true;
}

void CircleInvisibleWall::Delete()
{
	if (m_active == false)
	{
		return;
	}

	m_collision.Release();


	m_active = false;
}
/////////////////////////////////////////////////////////////////////


//if (g_pad[0]->IsPress(enButtonA) && g_pad[0]->IsPress(enButtonB))
//{
//	if (!m_colOn)
//	{
//		m_effect = NewGO<EffectEmitter>(0);
//		m_effect->Init(1);
//
//		m_effect->SetScale(Vector3(250.0f, 50.0f, 250.0f));
//		m_effect->SetPosition(m_player->GetPosition());
//		m_effect->Play();
//
//		m_colModel.SetPosition(m_player->GetPosition());
//		m_colModel.Update();
//
//		m_collision.CreateFromModel(m_colModel.GetModel(), m_colModel.GetModel().GetWorldMatrix());
//		m_colOn = true;
//	}
//}
//
//if (g_pad[0]->IsPress(enButtonX) && g_pad[0]->IsPress(enButtonY))
//{
//	if (m_colOn)
//	{
//		m_collision.Release();
//		m_colOn = false;
//	}
//}


