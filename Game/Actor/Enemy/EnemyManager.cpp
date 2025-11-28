#include "stdafx.h"
#include "EnemyManager.h"

#include "Actor\Enemy\Enemy.h"
#include "Actor\Enemy\EnemyMetaAi\EnemyMetaAi.h"

//インスタンス初期化
EnemyManager* EnemyManager::m_instance = nullptr;

EnemyManager::EnemyManager()
{
	m_enemyMetaAi = NewGO<EnemyMetaAi>(UpdateOrder::AI, "enemymetaai");
}

void EnemyManager::RequestSpawnEnemy(EnemyType type, const Vector3& spawnPoint)
{	
	auto newEnemy = m_enemyFactory.CreateEnemy(type);

	auto newAi = m_enemyAiFactory.GetInstance().Create(type, newEnemy->GetYakuzaStateMachine());

	newEnemy->SetAi(newAi.get());

	newEnemy->SetPosition(spawnPoint);

	EnemyPair newPair;

	newPair.m_enemy = newEnemy;
	newPair.m_enemyAi = newAi.get();
	newPair.m_type = type;

	m_enemyPairList.push_back(newPair);
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