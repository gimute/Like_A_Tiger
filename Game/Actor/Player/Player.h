#pragma once
#include "Actor\Character.h"
#include "Actor\Player\PlayerStateMachine.h"

//プレイヤーが操作するキャラクターを継承したクラス
class Player : public Character
{
public:
	enum PlayerAnimation
	{
		en_idle,
		en_walk,
		en_run,
		num
	};
private:
	AnimationData animationDataList[PlayerAnimation::num] =
	{
		//追加したい場合はここから下に伸ばす
		AnimationData{"Assets/modelData/Character/Survivalist/Animation/Idle.tka",true},
		AnimationData{"Assets/modelData/Character/Survivalist/Animation/Walking.tka",true},
		AnimationData{"Assets/modelData/Character/Survivalist/Animation/Run.tka",true}
	};
public:
	//コンストラクタ
	Player() = default;
	//デストラクタ
	~Player() = default;

	//スタート関数
	bool Start() override;
	//更新関数
	void Update() override;
	//描画関数
	void Render(RenderContext& rc) override;

	PlayerStateMachine* GetPlayerStateMachine()
	{
		return dynamic_cast<PlayerStateMachine*>(GetStateMachine());
	}
};