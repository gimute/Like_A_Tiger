#pragma once
#include "Actor\Actor.h"

class InvisibleWall;

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

	//場外に行かせないための透明壁、東西南北で4つ
	InvisibleWall* m_invisibleWall[4];
};

