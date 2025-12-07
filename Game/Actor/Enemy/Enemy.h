#pragma once
#include "Actor\Character.h"
#include "Actor\YakuzaComponents\YakuzaStateMachine.h"
#include "Actor\YakuzaComponents\YakuzaAnimationState.h"
#include "Actor\Enemy\EnemyAI\IEnemyAi.h"

class Enemy : public YakuzaCharacter
{
public:
	//エネミーのアニメーションは外部から変更できるようにしたいためここをいじる
private:
	IEnemyAi* m_hasAi;
public:
	//コンストラクタ
	Enemy() : YakuzaCharacter(this) {}
	//デストラクタ
	~Enemy() = default;

	//スタート関数
	bool Start() override;
	//更新関数
	void Update() override;
	//描画関数
	void Render(RenderContext& rc) override;

	//AI設定
	void SetAi(IEnemyAi* setAi) { m_hasAi = setAi; }

	void OnHit(const char* hitCollisionName, CollisionObject* pairCollision) override;
};

