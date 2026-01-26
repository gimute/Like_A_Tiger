#pragma once
#include <Actor/Actor.h>

class Item3DModelBase : public Actor
{
public:
	Item3DModelBase() {}
	virtual ~Item3DModelBase() {}

	virtual bool Start() = 0;
	virtual void Update() = 0;
	virtual void Render(RenderContext& rc) = 0;

	/** 座標設定 */
	void SetPosition(Vector3& pos)
	{
		m_transform.m_position = pos;
	}

	/** 回転設定 */
	void SetRotation(Quaternion& rot)
	{
		m_transform.m_rotation = rot;
	}

	/** 拡大率設計 */
	void SetScale(Vector3& scale)
	{
		m_transform.m_scale = scale;
	}

protected:
	Transform m_transform;

protected:
	/** 静的オブジェクトを付与 */
	PhysicsStaticObject m_physicsStaticObject;
	/** ゴーストオブジェクトを付与 */
	btGhostObject m_ghostObject;

	CollisionObject m_collision;
public:
	ModelRender m_modelRender;
	bool isVisible = true;
};




/*************************************************************/


class RecoveryItem3DModel :public Item3DModelBase
{
public:
	RecoveryItem3DModel();
	virtual ~RecoveryItem3DModel();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc);
};




/*************************************************************/


class WeaponItem3DModel : public Item3DModelBase
{
public:
	WeaponItem3DModel();
	virtual ~WeaponItem3DModel();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc);
};