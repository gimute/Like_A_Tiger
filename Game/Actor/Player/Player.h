#pragma once
#include "Actor\Character.h"
#include "Actor\YakuzaComponents\YakuzaStateMachine.h"
#include "Actor\YakuzaComponents\YakuzaAnimationState.h"

//プレイヤーが操作するキャラクターを継承したクラス
class Player : public Character
{
public:
	enum PlayerAnimation
	{
		en_punch_cross = YakuzaAnimation::en_num,
		en_kick_1,
		en_punch_1,
		num
	};
private:
	AnimationData animationDataList[PlayerAnimation::num] =
	{
		//追加したい場合はここから下に伸ばす
		AnimationData{"Assets/modelData/Character/Survivalist/Animation/Idle.tka",true},
		AnimationData{"Assets/modelData/Character/Survivalist/Animation/Walking.tka",true},
		AnimationData{"Assets/modelData/Character/Survivalist/Animation/Run.tka",true},
		AnimationData{"Assets/modelData/Character/Survivalist/Animation/Run.tka",true},//AimWalk予定
		AnimationData{"Assets/modelData/Character/Survivalist/Animation/Run.tka",true},//AimWalk予定
		AnimationData{"Assets/modelData/Character/Survivalist/Animation/Run.tka",true},//AimWalk予定
		AnimationData{"Assets/modelData/Character/Survivalist/Animation/Run.tka",true},//AimWalk予定
		AnimationData{"Assets/modelData/Character/Survivalist/Animation/Dodge_Forward.tka",false},
		AnimationData{"Assets/modelData/Character/Survivalist/Animation/Dodge_Right.tka",false},
		AnimationData{"Assets/modelData/Character/Survivalist/Animation/Dodge_Left.tka",false},
		AnimationData{"Assets/modelData/Character/Survivalist/Animation/Dodge_Back.tka",false},
		AnimationData{"Assets/modelData/Character/Survivalist/Animation/Guard.tka",true},
		AnimationData{"Assets/modelData/Character/Survivalist/Animation/Cross_Punch_EventTest_2.tka",false},
		AnimationData{"Assets/modelData/Character/Survivalist/Animation/Kick_1_R.tka",false},
		AnimationData{"Assets/modelData/Character/Survivalist/Animation/Punch_1_L.tka",false}
	};
public:
	//コンストラクタ
	Player();
	//デストラクタ
	~Player() = default;

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