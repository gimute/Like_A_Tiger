#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy()
{
	
}

bool Enemy::Start()
{
	InitAnimationClipList(m_maxAnimationNum,m_animationData.data());

	InitModelRender(m_modelFilePath);

	m_characterController.Init(10.0f, 20.0f, m_position);

	return true;
}

void Enemy::Update()
{
	GetYakuzaStateMachine()->UpdateStateMachine();

	m_characterController.SetPosition(m_position);;

	GetModelRender()->SetPosition(m_position);
	GetModelRender()->SetRotation(m_rotation);
	GetModelRender()->Update();
}

void Enemy::Render(RenderContext& rc)
{
	GetModelRender()->Draw(rc);
}