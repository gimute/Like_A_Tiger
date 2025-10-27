#include "stdafx.h"
#include "Player.h"

//スタート関数
bool Player::Start()
{

	MakeStateMachineUniquePtr<PlayerStateMachine>(this);

	InitAnimationClipList(PlayerAnimation::num, animationDataList);

	InitModelRender("");

	return true;
}

//アップデート関数
void Player::Update()
{
	//ステートマシン更新
	GetPlayerStateMachine()->UpdateStateMachine();
}

//描画関数
void Player::Render(RenderContext& rc)
{

}