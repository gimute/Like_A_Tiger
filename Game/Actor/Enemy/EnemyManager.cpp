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
	EnemyPair pair;

	auto newEnemy = m_enemyFactory.CreateEnemy(type);

	pair.m_enemy = newEnemy;

	pair.m_enemyAi = m_enemyAiFactory.GetInstance().Create(type, newEnemy->GetYakuzaStateMachine());

	newEnemy->SetAi(pair.m_enemyAi.get());

	newEnemy->SetPosition(spawnPoint);

	//m_enemyList.push_back(newEnemy);
	//m_enemyAiList.push_back(std::move(newAi));

	m_enemyPairList.push_back(std::move(pair));
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