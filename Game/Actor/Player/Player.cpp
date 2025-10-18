#include "stdafx.h"
#include "Player.h"

//スタート関数
bool Player::Start()
{

	m_stateMachine = std::make_unique<PlayerStateMachine>();

	return true;
}

//アップデート関数
void Player::Update()
{
	//ステートマシン更新
	m_stateMachine->UpdateStateMachine();
}

//描画関数
void Player::Render(RenderContext& rc)
{

}