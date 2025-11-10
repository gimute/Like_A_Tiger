#include "stdafx.h"
#include "Player.h"

//スタート関数
bool Player::Start()
{

	MakeStateMachineUniquePtr<YakuzaStateMachine>(this);

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

//void Player::ModelRotation()
//{
//	if (fabsf(GetYakuzaStateMachine()->GetMoveVec().x) >= 0.001f ||
//		fabsf(GetYakuzaStateMachine()->GetMoveVec().z) >= 0.001f)
//	{
//		//キャラクターの方向を変更
//		m_rotation.SetRotationYFromDirectionXZ(GetYakuzaStateMachine()->GetMoveVec());
//		m_modelRender.SetRotation(m_rotation);
//	}
//
//	m_forward = Vector3::Zero;
//	m_rotation.Apply(m_forward);
//}

//描画関数
void Player::Render(RenderContext& rc)
{
	GetModelRender()->Draw(rc);
}