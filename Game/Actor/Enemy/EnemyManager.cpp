#include "stdafx.h"
#include "EnemyManager.h"

#include "Actor\Enemy\Enemy.h"
#include "Actor\Enemy\EnemyMetaAi\EnemyMetaAi.h"

#include "Random.h"

//インスタンス初期化
EnemyManager* EnemyManager::m_instance = nullptr;

EnemyManager::EnemyManager()
{
	m_enemyMetaAi = NewGO<EnemyMetaAi>(UpdateOrder::AI, "enemymetaai");
}

void EnemyManager::RequestSpawnEnemy(EnemyType type, const Vector3& spawnPoint)
{	
	EnemyPair pair;

	auto newEnemy = m_enemyFactory.CreateEnemy(type);

	pair.m_enemy = newEnemy;

	pair.m_enemyAi = m_enemyAiFactory.GetInstance().Create(type, &newEnemy->GetYakuzaStateMachine());

	newEnemy->SetAi(pair.m_enemyAi.get());

	newEnemy->SetPosition(spawnPoint);

	m_enemyPairList.push_back(std::move(pair));
}

void EnemyManager::RequestSpawnEnemyGroup(int spawnNum, const Vector3& spawnPoint)
{
	EnemyGroup newGroup;

	for (int i = 0; i < spawnNum; i++)
	{
		EnemyPair pair;

		//タイプをランダム選定
		int randomType = 0;
		//Random::Range(EnemyType::en_normalYakuza)
		EnemyType type = static_cast<EnemyType>(randomType);

		auto newEnemy = m_enemyFactory.CreateEnemy(type);

		pair.m_enemy = newEnemy;

		pair.m_enemyAi = m_enemyAiFactory.GetInstance().Create(type, &newEnemy->GetYakuzaStateMachine());

		pair.m_enemyID = m_enemyIDCounter;

		newEnemy->SetAi(pair.m_enemyAi.get());

		//スポーン位置をランダム選定
		Vector3	randomSpawnPoint = GetRandomPointInRadius(spawnPoint, 100.0f);
	
		newEnemy->SetPosition(randomSpawnPoint);

		m_enemyPairList.push_back(std::move(pair));

		newGroup.m_enemyID.push_back(m_enemyIDCounter);

		m_enemyIDCounter++;
	}

	m_enemyGroupList.push_back(std::move(newGroup));
}

void EnemyManager::Update()
{
	UpdateTargetView();
}

void EnemyManager::UpdateTargetView()
{
	m_targetView.m_targetPosition = m_targetCharacter->GetPosition();

	m_targetView.m_targetForward = m_targetCharacter->GetForward();

	for (auto it = m_enemyPairList.begin();it != m_enemyPairList.end();)
	{
		it->m_enemyAi->UpdateTargetView(m_targetView);

		it++;
	}
}

Vector3 EnemyManager::GetRandomPointInRadius(const Vector3& point, float radius)
{
	Vector3 dir = Vector3::Zero;

	do
	{
		dir.x = Random::Range(-1.0f, 1.0f);
		dir.y = 0.0f;
		dir.z = Random::Range(-1.0f, 1.0f);
	} while (dir.LengthSq() > 1.0f);

	dir.Normalize();

	float distR = radius * std::cbrt(Random::Range(0.0f, 1.0f));

	return point + dir * distR;
}
