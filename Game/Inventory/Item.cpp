#include "stdafx.h"
#include "Item.h"


RecoveryItem3DModel::RecoveryItem3DModel()
{
}

RecoveryItem3DModel::~RecoveryItem3DModel()
{
}

bool RecoveryItem3DModel::Start()
{
	m_modelRender.Init("Assets/modelData/items/itemsRandom.tkm");
	m_modelRender.SetPosition(0.0f, 50.0f, 200.0f);
	m_modelRender.SetRotation(Quaternion::Identity);
	m_modelRender.SetScale(Vector3::One);
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(),m_modelRender.GetModel().GetWorldMatrix());

	//m_ghostObject.
	

	return true;
}

void RecoveryItem3DModel::Update()
{
	m_modelRender.Update();
}

void RecoveryItem3DModel::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}




/*************************************************************/


WeaponItem3DModel::WeaponItem3DModel()
{
}

WeaponItem3DModel::~WeaponItem3DModel()
{
}

bool WeaponItem3DModel::Start()
{
	return true;
}

void WeaponItem3DModel::Update()
{
}

void WeaponItem3DModel::Render(RenderContext& rc)
{
}