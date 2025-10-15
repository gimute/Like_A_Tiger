#pragma once
#include "Actor\Character.h"
#include "Actor\Player\PlayerStateMachine.h"

//プレイヤーが操作するキャラクターを継承したクラス
class Player : public Character
{
private:
	std::unique_ptr<PlayerStateMachine> m_stateMachine;
	
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
};

