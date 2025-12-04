#include "stdafx.h"
#include "Player.h"
#include "Actor\Player\PlayerAttackComboState.h"

//スタート関数
bool Player::Start()
{
	GetYakuzaStateMachine().InitAttackStateMachine(PlayerFirstAttackState::ID(), 0);

	GetYakuzaStateMachine().GetAttackStateMachine()->AddState<PlayerFirstAttackState>(GetYakuzaStateMachine().GetAttackStateMachine());
	GetYakuzaStateMachine().GetAttackStateMachine()->AddState<PlayerSecondAttackState>(GetYakuzaStateMachine().GetAttackStateMachine());
	GetYakuzaStateMachine().GetAttackStateMachine()->AddState<PlayerThirdAttackState>(GetYakuzaStateMachine().GetAttackStateMachine());
	GetYakuzaStateMachine().GetAttackStateMachine()->AddState<PlayerFirstFinalBlowState>(GetYakuzaStateMachine().GetAttackStateMachine());

	InitAnimationClipList(PlayerAnimation::num, animationDataList);

	InitModelRender("Assets/modelData/Character/Survivalist/Survivalist.tkm");

	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) { GetYakuzaStateMachine().OnAnimationEvent(clipName, eventName); });

	m_characterController.Init(10.0f, 40.0f, m_position);

	InitBodyCollision(this, "playerBodyCollision");

	return true;
}

//アップデート関数
void Player::Update()
{
	//ステートマシン更新
	GetYakuzaStateMachine().UpdateStateMachine();

	m_characterController.SetPosition(m_position);

	GetModelRender()->SetPosition(m_position);
	GetModelRender()->SetRotation(m_rotation);
	GetModelRender()->Update();

	//当たり判定の位置調整
	Vector3 positionCorrection = GetPosition();
	positionCorrection.y += 60.0f;

	m_bodyCollision->SetPosition(positionCorrection);
}

//描画関数
void Player::Render(RenderContext& rc)
{
	GetModelRender()->Draw(rc);
}

void Player::OnHit(const char* hitCollisionName, CollisionObject* pairCollision)
{
	if (hitCollisionName == "" &&
		pairCollision == m_bodyCollision)
	{
		GetYakuzaStateMachine().SetIsDamage(true);
	}
}