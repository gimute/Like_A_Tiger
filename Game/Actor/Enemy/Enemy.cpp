#include "stdafx.h"
#include "Enemy.h"
#include "Actor\YakuzaComponents\YakuzaCharacterDamageManager.h"

bool Enemy::Start()
{
	InitAnimationClipList(m_maxAnimationNum,m_animationData.data());

	InitModelRender(m_modelFilePath);

	m_characterController.Init(10.0f, 40.0f, m_position);

	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) { GetYakuzaStateMachine().OnAnimationEvent(clipName, eventName); });

	InitBodyCollision(this, "enemyBodyCollision");

	SetAttackCollisionName("EnemyAttack");

	return true;
}

void Enemy::Update()
{
	GetYakuzaStateMachine().UpdateStateMachine();

	m_characterController.SetPosition(m_position);;

	GetModelRender()->SetPosition(m_position);
	GetModelRender()->SetRotation(m_rotation);
	GetModelRender()->Update();

	//当たり判定の位置調整
	Vector3 positionCorrection = GetPosition();
	positionCorrection.y += 60.0f;

	m_bodyCollision->SetPosition(positionCorrection);

	//無敵時間のタイマー
	UpdateInvincibleTime();
}

void Enemy::Render(RenderContext& rc)
{
	GetModelRender()->Draw(rc);
}

void Enemy::OnHit(const char* hitCollisionName, CollisionObject* pairCollision)
{
	if (hitCollisionName == "PlayerAttack" &&
		pairCollision == m_bodyCollision)
	{
		//ダメージ取得
		float damage = YakuzaCharacterDamageManager::GetInstance()->GetPlayerYakuzaDamage();
		//ダメージ受ける
		TakeDamage(damage);
		//怯み処理
		GetYakuzaStateMachine().SetIsDamage(true);
	}

	if (hitCollisionName == "playerBodyCollision" &&
		pairCollision == m_attackCollision)
	{
		//攻撃コリジョン削除
		DeleteAttackCollision();
		//ダメージ取得
		float toPlayerDamage = GetYakuzaStateMachine().GetTypeSetAttackPower();
		//ダメージ送る
		YakuzaCharacterDamageManager::GetInstance()->SendPlayerYakuzaDamage(toPlayerDamage);
	}
}