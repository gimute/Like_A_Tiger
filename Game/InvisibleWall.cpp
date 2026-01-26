#include "stdafx.h"
#include "InvisibleWall.h"

///////////////////////////////////////////////////////////////////////////////

InvisibleWallShape* InvisibleWallShape::m_instance = nullptr;

InvisibleWallShape::InvisibleWallShape()
{
	ModelInitData initData;

	initData.m_tkmFilePath = "Assets/modelData/Collision/Box.tkm";
	m_shapeModels[Box].Init(initData);

	initData.m_tkmFilePath = "Assets/modelData/Collision/Circle.tkm";
	m_shapeModels[Circle].Init(initData);

}

///////////////////////////////////////////////////////////////////////////////

InvisibleWall::InvisibleWall()
{
	m_wallEffect.Init(3);
	m_lineEffect.Init(4);
}

InvisibleWall::~InvisibleWall()
{
}

bool InvisibleWall::Start()
{
	return true;
}

void InvisibleWall::Update()
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

	}
}

void InvisibleWall::Create(Vector3 pos, float width, float hight, float angle)
{
	if (m_active)
	{
		return;
	}

	//透明壁の指定されたサイズにするための拡大率
	float widthScale = width / BOX_SIZE;
	float hightScale = hight / BOX_SIZE;

	//Y軸の回転
	Quaternion YAngle;
	YAngle.SetRotation(g_vec3AxisY, angle);

	Model* boxModel = InvisibleWallShape::GetInstance().GetShapeModel(InvisibleWallShape::Box);

	//カメラと干渉しないよう縦のスケールは等倍
	//厚みがそのままだと分厚すぎるので半分
	boxModel->UpdateWorldMatrix(pos, YAngle, Vector3(widthScale, hightScale, 0.5f));

	//モデルから透明壁を生成
	m_collision.CreateFromModel(*boxModel, boxModel->GetWorldMatrix());

	//透明壁に沿う形のエフェクトを準備
	m_wallEffect.SetPosition(pos);
	m_wallEffect.SetRotation(YAngle);
	m_wallEffect.SetScale(Vector3(widthScale, 1.0f, 1.0f));
	m_wallEffect.Play();

	m_lineEffect.SetPosition(pos);
	m_lineEffect.SetRotation(YAngle);
	m_lineEffect.SetScale(Vector3(widthScale, 1.0f, 1.0f));
	m_lineEffect.Play();

	m_active = true;
}

void InvisibleWall::Delete()
{
	if (m_active == false)
	{
		return;
	}

	m_collision.Release();

	m_wallEffect.Stop();
	m_lineEffect.Stop();

	m_active = false;
}

///////////////////////////////////////////////////////////////////////////////


CircleInvisibleWall::CircleInvisibleWall()
{
	m_wallEffect.Init(1);
	m_lineEffect.Init(2);
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

	//コリジョンのユーザー属性を壁に設定
	m_collision.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Wall);

	//透明壁に沿う形のエフェクトを準備
	m_wallEffect.SetPosition(pos);
	m_wallEffect.SetScale(Vector3(scale,1.0f, scale));
	m_wallEffect.Play();

	m_lineEffect.SetPosition(pos);
	m_lineEffect.SetScale(Vector3(scale, 1.0f, scale));
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

	m_wallEffect.Stop();
	m_lineEffect.Stop();

	m_active = false;
}
/////////////////////////////////////////////////////////////////////

