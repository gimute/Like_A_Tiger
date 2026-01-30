#include "stdafx.h"
#include "ProtoStage.h"

#include "InvisibleWall.h"

bool ProtoStage::Start()
{

	m_modelRender.Init("Assets/modelData/Stage/Proto/Proto.tkm");

	m_modelRender.SetPosition({ 0.0f,0.0f,0.0f });

	//静的物理オブジェクトを作成
	physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	physicsStaticObject.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Wall);

	m_invisibleWall[0] = NewGO<InvisibleWall>(0);
	m_invisibleWall[0]->Create(Vector3(0.0f, 0.0f, -2000.0f), 13000.0f, 100.0f, 0.0f);
	m_invisibleWall[0]->SetPlayEffectFlag(false);
	m_invisibleWall[1] = NewGO<InvisibleWall>(0);
	m_invisibleWall[1]->Create(Vector3(0.0f, 0.0f, 5500.0f), 13000.0f, 100.0f, 0.0f);
	m_invisibleWall[1]->SetPlayEffectFlag(false);
	m_invisibleWall[2] = NewGO<InvisibleWall>(0);
	m_invisibleWall[2]->Create(Vector3(6000.0f, 0.0f, 1500.0f), 8000.0f, 100.0f, 180.0f);
	m_invisibleWall[2]->SetPlayEffectFlag(false);
	m_invisibleWall[3] = NewGO<InvisibleWall>(0);
	m_invisibleWall[3]->Create(Vector3(-6000.0f, 0.0f, 1500.0f), 8000.0f, 100.0f, 180.0f);
	m_invisibleWall[3]->SetPlayEffectFlag(false);

	return true;
}

void ProtoStage::Update()
{
	m_modelRender.Update();
}

void ProtoStage::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
