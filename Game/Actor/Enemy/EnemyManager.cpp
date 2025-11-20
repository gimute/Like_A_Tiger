#include "stdafx.h"
#include "EnemyManager.h"

#include "Actor\Enemy\Enemy.h"

//インスタンス初期化
EnemyManager* EnemyManager::m_instance = nullptr;

void EnemyManager::RequestSpawnEnemy(EnemyType type, const Vector3& spawnPoint)
{	
	auto newEnemy = m_enemyFactory.CreateEnemy(type);

	auto newAi = m_enemyAiFactory.GetInstance().Create(type, newEnemy->GetYakuzaStateMachine());

	newEnemy->SetAi(newAi.get());

	newEnemy->SetPosition(spawnPoint);

	m_aiList.push_back(std::move(newAi));
	m_enemyList.push_back(newEnemy);
}

void EnemyManager::Update()
{
	UpdateTargetView();
}

void EnemyManager::UpdateTargetView()
{
	m_targetView.m_targetPosition = m_targetCharacter->GetPosition();

	m_targetView.m_targetForward = m_targetCharacter->GetForward();

	for (auto it = m_aiList.begin();it != m_aiList.end();)
	{
		it->get()->UpdateTargetView(m_targetView);

		it++;
	}
}