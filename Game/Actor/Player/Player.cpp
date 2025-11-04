#include "stdafx.h"
#include "Player.h"

//スタート関数
bool Player::Start()
{

	MakeStateMachineUniquePtr<PlayerStateMachine>(this);

	InitAnimationClipList(PlayerAnimation::num, animationDataList);

	InitModelRender("Assets/modelData/Character/Survivalist/Survivalist.tkm");

	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) { GetPlayerStateMachine()->OnAnimationEvent(clipName, eventName); });

	m_characterController.Init(25.0f, 40.0f, m_position);
	
	return true;
}

//アップデート関数
void Player::Update()
{
	//ステートマシン更新
	GetPlayerStateMachine()->UpdateStateMachine();

	m_characterController.SetPosition(m_position);

	GetModelRender()->SetPosition(m_position);
	ModelRotation();
	GetModelRender()->Update();
}

void Player::ModelRotation()
{
	if (fabsf(GetPlayerStateMachine()->GetPlayerMoveVec().x) >= 0.001f ||
		fabsf(GetPlayerStateMachine()->GetPlayerMoveVec().z) >= 0.001f)
	{
		//キャラクターの方向を変更
		m_rotation.SetRotationYFromDirectionXZ(GetPlayerStateMachine()->GetPlayerMoveVec());
		m_modelRender.SetRotation(m_rotation);
	}

	m_forward = Vector3::Zero;
	m_rotation.Apply(m_forward);
}

//描画関数
void Player::Render(RenderContext& rc)
{
	GetModelRender()->Draw(rc);
}