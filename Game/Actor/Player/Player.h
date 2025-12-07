#pragma once
#include "Actor\Character.h"
#include "Actor\YakuzaComponents\YakuzaStateMachine.h"
#include "Actor\YakuzaComponents\YakuzaAnimationState.h"

//プレイヤーが操作するキャラクターを継承したクラス
class Player : public YakuzaCharacter
{
public:
	//コンストラクタ
	Player() : YakuzaCharacter(this) {}
	//デストラクタ
	~Player() = default;

	//スタート関数
	bool Start() override;
	//更新関数
	void Update() override;
	//描画関数
	void Render(RenderContext& rc) override;

	void OnHit(const char* hitCollisionName, CollisionObject* pairCollision) override;
};