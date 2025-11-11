#include "stdafx.h"
#include "ProtoStage.h"

bool ProtoStage::Start()
{

	m_modelRender.Init("Assets/modelData/Stage/Proto/Proto.tkm");

	m_modelRender.SetPosition({ 0.0f,0.0f,0.0f });

	//静的物理オブジェクトを作成
	physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

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
