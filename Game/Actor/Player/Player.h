#pragma once
#include "Actor\Character.h"
#include "Actor\Player\PlayerStateMachine.h"

//プレイヤーが操作するキャラクターを継承したクラス
class Player : public Character
{
	enum PlayerAnimation
	{
		en_idle,
		en_run,
		en_attack1,
		en_attack2,
		num
	};

	AnimationData animationDataList[PlayerAnimation::num] =
	{
		//追加したい場合はここから下に伸ばす
		AnimationData{"debug_file1",true},
		AnimationData{"debug_file2",true},
		AnimationData{"debug_file3",true},
		AnimationData{"debug_file4",true},
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