#pragma once

#include "ActorStatus.h"

//見た目が存在するゲームオブジェクトの規定クラス
class Actor : public IGameObject
{
public:

	//参考にしたプログラムではココがまとめられたTransformというクラスになっていた
	//面白そうなので後で見てみる

	/// <summary>
	/// 位置
	/// </summary>
	Vector3 m_position = Vector3::Zero;	
	/// <summary>
	/// 回転
	/// </summary>
	Quaternion m_rotation = Quaternion::Identity;	
	/// <summary>
	/// 拡大率
	/// </summary>
	Vector3 m_scale = Vector3::One;

protected:
	//モデル描画機能
	ModelRender m_modelRender;
	//ステータス用変数
	ActorStatus* m_actorStatus = nullptr;

public:
	//コンストラクタ
	Actor() = default;
	//デストラクタ
	~Actor() = default;

	//スタート関数
	virtual bool Start() override;

	//更新関数
	virtual void Update() override;

	//描画関数
	virtual void Render(RenderContext& rc) override;

public: 
	//モデル描画機能を取得する関数
	ModelRender* GetModelRender()
	{
		return &m_modelRender;
	}
};

