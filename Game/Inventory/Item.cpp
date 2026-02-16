#include "stdafx.h"
#include "Item.h"
#include "Inventory/Inventory.h"
#include "Actor/Player/Player.h"

namespace {
	constexpr const char* ITEMS_RANDOM_PATH = "Assets/modelData/items/itemsRandom.tkm";

	const float MIN_SCALE = 0.0000001f;
	const float MAX_SCALE = 1.0f;

	static constexpr float COOLDOWN_DURATION = 5.0f;
}

RecoveryItem3DModel::RecoveryItem3DModel()
{
}

RecoveryItem3DModel::~RecoveryItem3DModel()
{
}

bool RecoveryItem3DModel::Start()
{
	m_transform.m_localPosition.y += 50.0f;
	m_transform.m_localRotation = Quaternion::Identity;
	m_transform.m_localScale = Vector3(m_scale);

	m_modelRender.Init(ITEMS_RANDOM_PATH);
	m_modelRender.SetPosition(m_transform.m_localPosition);
	m_modelRender.SetRotation(m_transform.m_localRotation);
	m_modelRender.SetScale(m_transform.m_localScale);
	
	m_collision = NewGO<CollisionObject>(0, "collision");
	m_collision->CreateSphere(
		GetPosition(),
		GetRotation(),
		25.0f,
		this
	);
	
	return true;
}

void RecoveryItem3DModel::Update()
{
	/** クールタイムの更新 */
	if (m_coolDownTimer > 0.0f) {
		m_coolDownTimer -= g_gameTime->GetFrameDeltaTime();
		if (m_coolDownTimer <= 0.0f) {
			m_coolDownTimer = 0.0f;
			/** クールタイム終了時にリセット*/
			m_collectedItem = false;
		}
	}
	CheckPlayerCollision();
	UpdateScaleAnimation();

	if (m_collision) {
		m_collision->SetPosition(m_transform.m_localPosition);
	}
	m_modelRender.SetScale(m_transform.m_localScale);
	m_modelRender.Update();
}

void RecoveryItem3DModel::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}

void RecoveryItem3DModel::CheckPlayerCollision()
{
	/** 取得済みか、クールタイム中なら何もしない */
	if (m_collectedItem || IsOnCoolDown()) return;

	bool isNeared = m_isPlayerNear;
	m_isPlayerNear = ItemCollisionManager::GetInstance()->CheckItemPlayerCollision(this, m_radius);

	/** 状態が変化した時アニメーション再生 */
	if (m_isPlayerNear != isNeared) {
		if (m_isPlayerNear) {
			PlayShrinkAnimation();
			GrantRandomItem();
			m_collectedItem = true;
			m_coolDownTimer = COOLDOWN_DURATION;
		}
		else {
			PlayExpandAnimation();
		}
	}
}

void RecoveryItem3DModel::PlayShrinkAnimation()
{
	m_targetScale = MIN_SCALE;
	m_isAnimating = true;
}

void RecoveryItem3DModel::PlayExpandAnimation()
{
	m_targetScale = MAX_SCALE;
	m_isAnimating = true;
}

void RecoveryItem3DModel::UpdateScaleAnimation()
{
	if (!m_isAnimating) return;

	float currentScale = m_transform.m_localScale.x;
	float deltaTime = g_gameTime->GetFrameDeltaTime();

	//目標スケールに向かって補間
	float newScale = currentScale;

	if (currentScale < m_targetScale) {
		//拡大中
		newScale = currentScale + m_animationSpeed * deltaTime;
		if (newScale >= m_targetScale) {
			newScale = m_targetScale;
			m_isAnimating = false;
		}
	}
	else if (currentScale > m_targetScale) {
		//縮小中
		newScale = currentScale - m_animationSpeed * deltaTime;
		if (newScale <= m_targetScale) {
			newScale = m_targetScale;
			m_isAnimating = false;
		}
	}
	else {
		m_isAnimating = false;
	}
	m_transform.m_localScale = Vector3(newScale, newScale, newScale);
}

void RecoveryItem3DModel::GrantRandomItem()
{
	Inventory* inventory = Inventory::GetInstance();
	if (!inventory) {
		return;
	}

	/** 3種類の回復アイテムからランダムで選択 */
	int randomIndex = rand() % 3;
	EnItemType itemType = static_cast<EnItemType>(enItemType_Onigiri + randomIndex);
	/** インベントリにアイテムを追加 */
	int result = inventory->AddItem(itemType);
}




/*************************************************************/


ItemCollisionManager* ItemCollisionManager::m_instance = nullptr;

bool ItemCollisionManager::CheckItemPlayerCollision(Item3DModelBase* item3DModeBase, float radius) {
	if (!m_playerPtr || !item3DModeBase) {
		return false;
	}

	Vector3 playerPos = m_playerPtr->GetPosition();
	Vector3 itemPos = item3DModeBase->GetLocalPosition();

	Vector3 diff = playerPos - itemPos;
	float distance = diff.LengthSq();

	//デバッグ出力
	K2_LOG("Distance %.2f\n", distance);

	const float collisionRadius = 90.0f;
	return (distance < collisionRadius * collisionRadius);
}