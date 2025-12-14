#include "stdafx.h"
#include "EnemyManager.h"

#include <unordered_set>

#include "Actor\Enemy\Enemy.h"
#include "Actor\Enemy\EnemyMetaAi\EnemyMetaAi.h"

#include "Random.h"

//インスタンス初期化
EnemyManager* EnemyManager::m_instance = nullptr;

EnemyManager::EnemyManager()
{
	m_enemyMetaAi = NewGO<EnemyMetaAi>(UpdateOrder::AI, "enemymetaai");
}

void EnemyManager::RequestSpawnEnemy(EnemyYakuzaType type, const Vector3& spawnPoint)
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
		//Random::Range(EnemyYakuzaType::en_normalYakuza)
		EnemyYakuzaType type = static_cast<EnemyYakuzaType>(randomType);

		auto newEnemy = m_enemyFactory.CreateEnemy(type);

		pair.m_enemy = newEnemy;

		pair.m_enemyAi = m_enemyAiFactory.GetInstance().Create(type, &newEnemy->GetYakuzaStateMachine());

		pair.m_enemyID = m_enemyIDCounter;

		//仮名なので注意
		pair.m_enemyName = EnemyTempNames[i];

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
	//外部用のエネミー情報リスト
	UpdateEnemyDataSet();

	UpdateTargetView();

	//テスト

	auto& testlist = GetEnemyGroupList();

	for(auto & ptr : testlist)
	{

		if (ptr.isInBattle)
		{
			bool istrue = true;
		}
		else
		{
			bool isfalse = false;
		}

	}

}

void EnemyManager::UpdateEnemyDataSet()
{
	auto& enemyGroupList = EnemyManager::GetInstance()->GetEnemyGroupList();
	auto& enemyPairList = EnemyManager::GetInstance()->GetEnemyPairList();

	if (enemyGroupList.empty())
	{
		m_enemyInfoList.clear();
		return;
	}

	auto targetView = EnemyManager::GetInstance()->GetTargetView();

	std::unordered_set<int> currentGroupIds;

	for (int groupId = 0; groupId < enemyGroupList.size(); ++groupId)
	{
		auto& group = enemyGroupList[groupId];
		currentGroupIds.insert(groupId);

		//グループがすでにあるかを探索
		EnemyInfoGroupe* existGroup = nullptr;

		for (auto& g : m_enemyInfoList)
		{
			if (g.m_groupId == groupId)
			{
				existGroup = &g;

				break;
			}
		}

		//このグループが戦闘中かどうか
		bool isInBattle = false;

		//無いなら新規追加
		if (!existGroup)
		{
			EnemyInfoGroupe newGroup;
			newGroup.m_groupId = groupId;
			m_enemyInfoList.push_back(newGroup);
			existGroup = &m_enemyInfoList.back();
		}
		//新規でないならグループ情報を更新
		else
		{
			existGroup[groupId].m_inBattle = enemyGroupList[groupId].isInBattle;
		}

		//グループ内部の更新
		existGroup->m_enemyAiInfoList.clear();

		//EnemyIdごとにEnemyMemberInfowo作成
		for (auto& id : group.m_enemyID)
		{
			EnemyPair* pair = nullptr;

			for (auto& enemy : enemyPairList)
			{
				if (enemy.m_enemyID == id)
				{
					pair = &enemy;

					break;
				}
			}

			if (!pair)
			{
				continue;
			}

			Enemy* enemyInst = pair->m_enemy;
			IEnemyAi* aiInst = pair->m_enemyAi.get();
			EnemyYakuzaType type = pair->m_type;
			Vector3 enemyPos = pair->m_enemy->GetPosition();
			const char* enemyName = pair->m_enemyName;
			bool isActive = true;

			EnemyMemberInfo info(
				enemyInst,
				aiInst,
				type,
				enemyPos,
				enemyName,
				isActive
			);

			existGroup->m_enemyAiInfoList.emplace_back(info);
		}
	}

	//今フレームに無いGroupIdは削除
	m_enemyInfoList.erase(
		std::remove_if(
			m_enemyInfoList.begin(),
			m_enemyInfoList.end(),
			[&](const EnemyInfoGroupe& g)
			{
				return currentGroupIds.count(g.m_groupId) == 0;
			}
		),
		m_enemyInfoList.end()
	);
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
