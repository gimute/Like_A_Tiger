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
		en_punch_cross,
		en_kick_1,
		num
	};
private:
	AnimationData animationDataList[PlayerAnimation::num] =
	{
		//追加したい場合はここから下に伸ばす
		AnimationData{"Assets/modelData/Character/Survivalist/Animation/Idle.tka",true},
		AnimationData{"Assets/modelData/Character/Survivalist/Animation/Walking.tka",true},
		AnimationData{"Assets/modelData/Character/Survivalist/Animation/Run.tka",true},
		AnimationData{"Assets/modelData/Character/Survivalist/Animation/Cross_Punch_EventTest_2.tka",false},
		AnimationData{"Assets/modelData/Character/Survivalist/Animation/Kick_1_R.tka",false}
	};
	float comboWindowTimeList[PlayerAnimation::num] =
	{
		0.0f,
		0.0f,
		0.0f,
		0.5f,
		0.5f,
	};
private:
	//プレイヤーの正面方向
	Vector3 m_forward = Vector3::AxisZ;
public:
	//コンストラクタ
	Player() = default;
	//デストラクタ
	~Player() = default;

	//スタート関数
	bool Start() override;
	//更新関数
	void Update() override;
	//モデル回転
	void ModelRotation();
	//描画関数
	void Render(RenderContext& rc) override;

	PlayerStateMachine* GetPlayerStateMachine()
	{
		return dynamic_cast<PlayerStateMachine*>(GetStateMachine());
	}

	inline void SetPlayerForward(const Vector3& forward) { m_forward = forward; }

	inline const Vector3& GetPlayerForward() { return m_forward; }

	inline CharacterController* GetCharacterController() { return &m_characterController; }
};