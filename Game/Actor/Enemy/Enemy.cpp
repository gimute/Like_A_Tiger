#include "stdafx.h"
#include "Enemy.h"

bool Enemy::Start()
{
	InitAnimationClipList(m_maxAnimationNum,m_animationData.data());

	InitModelRender(m_modelFilePath);

	m_characterController.Init(10.0f, 40.0f, m_position);

	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) { GetYakuzaStateMachine().OnAnimationEvent(clipName, eventName); });

	InitBodyCollision(this, "testEnemy");

	return true;
}

void Enemy::Update()
{
	GetYakuzaStateMachine().UpdateStateMachine();

	m_characterController.SetPosition(m_position);;

	GetModelRender()->SetPosition(m_position);
	GetModelRender()->SetRotation(m_rotation);
	GetModelRender()->Update();

	//“–‚½‚è”»’è‚ÌˆÊ’u’²®
	Vector3 positionCorrection = GetPosition();
	positionCorrection.y += 60.0f;

	m_bodyCollision->SetPosition(positionCorrection);
}

void Enemy::Render(RenderContext& rc)
{
	GetModelRender()->Draw(rc);
}