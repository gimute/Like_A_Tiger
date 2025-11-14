#pragma once
#include "Actor\Character.h"
#include "Actor\YakuzaComponents\YakuzaStateMachine.h"
#include "Actor\YakuzaComponents\YakuzaAnimationState.h"

class Enemy : public Character
{
public:
	//エネミーのアニメーションは外部から変更できるようにしたいためここをいじる
private:



public:
	//コンストラクタ
	Enemy();
	//デストラクタ
	~Enemy() = default;

	//スタート関数
	bool Start() override;
	//更新関数
	void Update() override;
	//描画関数
	void Render(RenderContext& rc) override;

	YakuzaStateMachine* GetYakuzaStateMachine()
	{
		return dynamic_cast<YakuzaStateMachine*>(GetStateMachine());
	}
};

