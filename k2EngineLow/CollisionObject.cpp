#include "k2EnginelowPreCompile.h"
#include "CollisionObject.h"

namespace nsK2EngineLow {
	CollisionObject::CollisionObject()
	{

	}

	CollisionObject::~CollisionObject()
	{
		if (g_collisionObjectManager) {
			g_collisionObjectManager->RemoveCollisionObject(this);
		}
	}

	bool CollisionObject::Start()
	{
		g_collisionObjectManager->AddCollisionObject(this);
		return true;
	}

	void CollisionObject::Update()
	{
		if (m_isEnableAutoDelete == false)
		{
			return;
		}


		if (m_timer > m_timeLimit + 0000.1f)
		{
			DeleteGO(this);
		}
		m_timer += g_gameTime->GetFrameDeltaTime();
	}


	CollisionObjectManager::CollisionObjectManager()
	{

	}

	CollisionObjectManager::~CollisionObjectManager()
	{
		m_collisionObjectVector.clear();
	}

	void CollisionObjectManager::Update()
	{
		CollisionPair tmpPair;

		//重なっているコリジョン検索、総当たり
		for (int i = 0; i < m_collisionObjectVector.size(); i++)
		{
			for (int j = i + 1; j < m_collisionObjectVector.size(); j++)
			{
				if (m_collisionObjectVector[i]->IsHit(m_collisionObjectVector[j]))
				{
					//重なっているのを見つけたら登録
					tmpPair.m_collisionA = m_collisionObjectVector[i];
					tmpPair.m_collisionB = m_collisionObjectVector[j];
					m_overlapCollisionPair.push_back(tmpPair);
				}
			}
		}

		//TODO:重なってるコリジョンの持ち主のHit処理呼び出し
		for (auto& pair : m_overlapCollisionPair)
		{
			pair.m_collisionA->GetOwner()->OnHit(pair.m_collisionB->GetName(), pair.m_collisionA);
			pair.m_collisionB->GetOwner()->OnHit(pair.m_collisionA->GetName(), pair.m_collisionB);
		}

		m_overlapCollisionPair.clear();
	}
	
}
