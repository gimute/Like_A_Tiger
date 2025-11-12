#pragma once
#include "Actor\Actor.h"

class ProtoStage : 
	public Actor {
public:
	//コンストラクタ
	ProtoStage() = default;
	//デストラクタ
	~ProtoStage() = default;
	//スタート関数
	bool Start() override;
	//アップデート関数
	void Update() override;
	//レンダー関数
	void Render(RenderContext& rc);
	//静的オブジェクトを付与
	PhysicsStaticObject		physicsStaticObject;	

};

