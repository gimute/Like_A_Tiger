#include "stdafx.h"
#include "Enemy.h"
#include "Actor\YakuzaComponents\YakuzaCharacterDamageManager.h"
#include "Actor\Enemy\EnemyManager.h"

bool Enemy::Start()
{
	InitAnimationClipList(m_maxAnimationNum,m_animationData.data());

	InitModelRender(m_modelFilePath);

	m_characterController.Init(10.0f, 40.0f, m_position);

	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) { GetYakuzaStateMachine().OnAnimationEvent(clipName, eventName); });

	InitBodyCollision(this, "enemyBodyCollision");

	SetAttackCollisionName("EnemyAttack");

	SetGrabCollisionName("EnemyGrab");

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
		YakuzaDamageDatas damage = YakuzaCharacterDamageManager::GetInstance()->GetPlayerYakuzaDamage();

		//ダメージを受ける
		YakuzaCharacterDamageManager::GetInstance()->SendEnemyYakuzaDamage(this, damage);
	}

	if (hitCollisionName == "playerBodyCollision" &&
		pairCollision == m_attackCollision)
	{
		//攻撃コリジョン削除
		DeleteAttackCollision();
		//ダメージ取得
		YakuzaDamageDatas toPlayerDamage = GetYakuzaStateMachine().GetTypeSetAttackPower();
		//ダメージ送る
		YakuzaCharacterDamageManager::GetInstance()->SendPlayerYakuzaDamage(toPlayerDamage,GetPosition());
	}

	if (hitCollisionName == "GrabPlayer" &&
		pairCollision == m_bodyCollision)
	{
		//掴まれた
		GetYakuzaStateMachine().GrabBedStart(
			YakuzaCharacterDamageManager::GetInstance()->SendPlayerGrabEnemyYakuza(this)
		);
	}

	if (hitCollisionName == "playerBodyCollision" &&
		pairCollision == m_grabCollision)
	{
		//掴んだ
		GetYakuzaStateMachine().GrabStart(
			YakuzaCharacterDamageManager::GetInstance()->SendEnemyGrabPlayerYakuza(this)
		);
	}
}

void Enemy::YakuzaCharacterDeadProcces()
{
	//敵の死亡処理

	//マネージャーに死んだことを伝える
	EnemyManager::GetInstance()->RequestDeadEnemyProcces(*this);

	//自身を削除する
	DeleteGO(this);
}