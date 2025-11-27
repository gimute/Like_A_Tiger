#include "stdafx.h"
#include "EnemySystem.h"

#include "Actor\Enemy\EnemyManager.h"

bool EnemySystem::Start()
{
	return true;
}

void EnemySystem::Update()
{
	//‘S‚Ä‚ÌAI‚ÌXV
	AllAiUpdate();
}

void EnemySystem::AllAiUpdate()
{
	auto& allEnemyAi = EnemyManager::GetInstance()->GetAiList();

	for (auto it = allEnemyAi.begin();it != allEnemyAi.end();it++)
	{
		it->get()->UpdateStateMachine();
	}
}