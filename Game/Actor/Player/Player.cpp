#include "stdafx.h"
#include "Player.h"
#include "Actor\Player\PlayerAttackComboState.h"

//コンストラクタ
Player::Player()
{
	MakeStateMachineUniquePtr<YakuzaStateMachine>(this);
}

//スタート関数
bool Player::Start()
{
	GetYakuzaStateMachine()->InitAttackStateMachine(PlayerFirstAttackState::ID(), 0);

	GetYakuzaStateMachine()->GetAttackStateMachine()->AddState<PlayerFirstAttackState>(GetYakuzaStateMachine()->GetAttackStateMachine());
	GetYakuzaStateMachine()->GetAttackStateMachine()->AddState<PlayerSecondAttackState>(GetYakuzaStateMachine()->GetAttackStateMachine());
	GetYakuzaStateMachine()->GetAttackStateMachine()->AddState<PlayerThirdAttackState>(GetYakuzaStateMachine()->GetAttackStateMachine());
	GetYakuzaStateMachine()->GetAttackStateMachine()->AddState<PlayerFirstFinalBlowState>(GetYakuzaStateMachine()->GetAttackStateMachine());

	InitAnimationClipList(PlayerAnimation::num, animationDataList);

	InitModelRender("Assets/modelData/Character/Survivalist/Survivalist.tkm");

	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) { GetYakuzaStateMachine()->OnAnimationEvent(clipName, eventName); });

	m_characterController.Init(25.0f, 40.0f, m_position);
	
	return true;
}

//アップデート関数
void Player::Update()
{
	//ステートマシン更新
	GetYakuzaStateMachine()->UpdateStateMachine();

	m_characterController.SetPosition(m_position);

	GetModelRender()->SetPosition(m_position);
	GetModelRender()->SetRotation(m_rotation);
	GetModelRender()->Update();
}

//描画関数
void Player::Render(RenderContext& rc)
{
	GetModelRender()->Draw(rc);
}