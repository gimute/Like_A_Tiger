#pragma once
#include <Actor/Actor.h>

class Player;
class Item3DModelBase : public Actor
{
public:
	Item3DModelBase() {}
	virtual ~Item3DModelBase() {}

	virtual bool Start() = 0;
	virtual void Update() = 0;
	virtual void Render(RenderContext& rc) = 0;

	/** プレイヤーとの当たり判定を確認 */
	virtual void CheckPlayerCollision() = 0;
	/** 縮むアニメーションを再生 */
	virtual void PlayShrinkAnimation() = 0;
	/** 拡張するアニメーションを再生 */
	virtual void PlayExpandAnimation() = 0;
	/** スケールアニメーションを再生 */
	virtual void UpdateScaleAnimation() = 0;
	/** ランダムでアイテムを付与 */
	virtual void GrantRandomItem() = 0;

	/** 座標設定 */
	void SetPosition(Vector3& pos)
	{
		m_transform.m_localPosition = pos;
	}

	/** 回転設定 */
	void SetRotation(Quaternion& rot)
	{
		m_transform.m_localRotation = rot;
	}

	/** 拡大率設計 */
	void SetScale(Vector3& scale)
	{
		m_transform.m_localScale = scale;
	}

	Vector3 GetLocalPosition() {
		return m_transform.m_localPosition;
	}

	/**	アイテムが取得済みか */
	bool IsItemCollected() const {
		return m_collectedItem;
	}

	/** クールタイム中か */
	bool IsOnCoolDown() const {
		return m_coolDownTimer > 0.0f;
	}

protected:
	Transform m_transform;

protected:
	CollisionObject* m_collision;
	ModelRender m_modelRender;

	Vector3 m_scale = Vector3::One;
	Quaternion m_rotation = Quaternion::Identity;

	float m_radius = 50.0f;
	/** 目標のスケール */
	float m_targetScale = 1.0f;
	/** アニメーション速度 */
	float m_animationSpeed = 3.0f;
	/** クールタイムタイマー */
	float m_coolDownTimer = 0.0f;


	bool m_isVisible = true;
	bool m_isPlayerNear = false;
	/** アニメーション中か */
	bool m_isAnimating = false;
	/** アイテムが取得済みか */
	bool m_collectedItem = false;
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

	/** プレイヤーとの当たり判定を確認 */
	void CheckPlayerCollision() override;
	/** 縮むアニメーションを再生 */
	void PlayShrinkAnimation() override;
	/** 拡張するアニメーションを再生 */
	void PlayExpandAnimation() override;
	/** スケールアニメーションを再生 */
	void UpdateScaleAnimation() override;
	/** ランダムでアイテムを付与 */
	void GrantRandomItem() override;
};




/*************************************************************/


class ItemCollisionManager {
private:
	ItemCollisionManager() = default;
	~ItemCollisionManager() = default;

public:
	static ItemCollisionManager* GetInstance() {
		if (!m_instance) {
			m_instance = new ItemCollisionManager();
		}
		return m_instance;
	}

	static void DeleteInstance() {
		delete m_instance;
		m_instance = nullptr;
	}

	void SetPlayerPtr(Player* player) {
		m_playerPtr = player;
	}

	Player* GetPlayerPtr() const {
		return m_playerPtr;
	}

public:
	/** アイテムとPlayerとの衝突判定 */
	bool CheckItemPlayerCollision(Item3DModelBase* item3DModelBase, float radius);

private:
	static ItemCollisionManager* m_instance;
	Player* m_playerPtr = nullptr;
};