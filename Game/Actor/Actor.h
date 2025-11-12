#pragma once

#include "ActorStatus.h"

//見た目が存在するゲームオブジェクトの規定クラス
class Actor : public IGameObject
{
protected:

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
	/// <summary>
	/// 正面ベクトル
	/// </summary>
	Vector3 m_forward = Vector3::AxisZ;


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

	inline void SetPosition(const Vector3& pos) { m_position = pos; }

	inline const Vector3& GetPosition() { return m_position; }

	inline void SetRotation(const Quaternion& rot) { m_rotation = rot; }

	inline  Quaternion& GetRotation() { return m_rotation; }

	inline void SetScale(const Vector3& scale) { m_scale = scale; }

	inline const Vector3& GetScale() { return m_scale; }

	inline void SetForward(const Vector3& forward) { m_forward = forward; }

	inline Vector3& GetForward() { return m_forward; }
};

