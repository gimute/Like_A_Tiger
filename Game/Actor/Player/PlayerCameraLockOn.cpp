#include "stdafx.h"
#include "PlayerCameraLockOn.h"

#include "Actor\Enemy\Enemy.h"
#include "Actor\Enemy\EnemyManager.h"

PlayerCameraLockOn* PlayerCameraLockOn::m_instance = nullptr;

void PlayerCameraLockOn::InitPlayerLockOn()
{

}

bool PlayerCameraLockOn::CalcCameraLockOn(
	Vector3& outValue,
	const Vector3& playerPos
)
{
	Vector3 finalCameraForward = Vector3::Zero;
	Enemy* nextlockOnEnemy = nullptr;

	if (m_lockOnInputState == LockOnInputDir::enLockOnIn)
	{
		nextlockOnEnemy = LockOnStart(
			g_camera3D->GetPosition(),
			g_camera3D->GetForward()
		);

		m_lockOnInputState = LockOnInputDir::enInputNone;
	}
	else if (m_lockOnInputState == LockOnInputDir::enLockOnRight)
	{
		nextlockOnEnemy = LockOnSwitch(
			g_camera3D->GetPosition(),
			g_camera3D->GetRight(),
			m_lockOnCurrent,
			true
		);

		m_lockOnInputState = LockOnInputDir::enInputNone;
	}
	else if (m_lockOnInputState == LockOnInputDir::enLockOnLeft)
	{
		nextlockOnEnemy = LockOnSwitch(
			g_camera3D->GetPosition(),
			g_camera3D->GetRight(),
			m_lockOnCurrent,
			false
		);

		m_lockOnInputState = LockOnInputDir::enInputNone;
	}

	if (m_lockOnCurrent && m_lockOnCurrent->GetYakuzaStateMachine().GetIsDead())
	{
		EndLockOn();

		return false;
	}

	if (nextlockOnEnemy && m_lockOnCurrent != nextlockOnEnemy)
	{
		m_lockOnCurrent = nextlockOnEnemy;
	}
	//ロックオン対象が存在していればそのまま対象の方向にロックオン
	if (m_lockOnCurrent)
	{
		//ロックオン中の敵の向きへカメラを向けるためにベクトル計算
		finalCameraForward = m_lockOnCurrent->GetPosition() - playerPos;
		finalCameraForward.Normalize();
	}

	outValue = finalCameraForward;

	return true;
}

Enemy* PlayerCameraLockOn::LockOnStart(
	const Vector3& camPos,
	const Vector3& camForward
)
{
	const float fovHalfAngle = DirectX::XMConvertToRadians(45.0f);
	const float minDot = cosf(fovHalfAngle);

	return SearchLockOnEnemy([&](Enemy* enemy)
		{
			Vector3 toEnemy = enemy->GetPosition() - camPos;
			toEnemy.y = 0.0f;

			float lengthSq = toEnemy.LengthSq();
			if (lengthSq < 0.0001f)
			{
				return -FLT_MAX;
			}

			toEnemy.Normalize();

			float dot = toEnemy.Dot(camForward);

			if (dot < minDot)
			{
				return -FLT_MAX;
			}

			return dot;
		}
	);
}

Enemy* PlayerCameraLockOn::LockOnSwitch(
	const Vector3& camPos,
	const Vector3& camRight,
	Enemy* currentEnemy,
	bool inputRight
)
{
	return SearchLockOnEnemy([&](Enemy* enemy)
		{
			if (enemy == currentEnemy)
			{
				return -FLT_MAX;
			}

			Vector3 toEnemy = enemy->GetPosition() - camPos;
			toEnemy.y = 0.0f;
			toEnemy.Normalize();

			float side = camRight.Dot(toEnemy);

			Vector3 toCurrentEnemy = currentEnemy->GetPosition() - camPos;
			toCurrentEnemy.y = 0.0f;
			toCurrentEnemy.Normalize();

			float currentSide = camRight.Dot(toCurrentEnemy);

			if (inputRight && side <= currentSide)
			{
				return -FLT_MAX;
			}

			if (!inputRight && side >= currentSide)
			{
				return -FLT_MAX;
			}

			return -fabsf(side - currentSide);
		}
	);
}

Enemy* PlayerCameraLockOn::SearchLockOnEnemy(std::function<float(Enemy*)> scoringFunc)
{
	if (!m_inBattleEnemys)
	{
		return nullptr;
	}

	Enemy* bestEnemy = nullptr;
	float bestScore = -FLT_MAX;

	for (auto& enemy : m_inBattleEnemys->m_enemyAiInfoList)
	{
		Enemy* loadEnemy = enemy.m_enemy;

		if (loadEnemy->GetYakuzaStateMachine().GetIsDead())
		{
			continue;
		}

		float score = scoringFunc(loadEnemy);

		if (score > bestScore)
		{
			bestScore = score;
			bestEnemy = loadEnemy;
		}
	}

	return bestEnemy;
}