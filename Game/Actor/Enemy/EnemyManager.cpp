#include "stdafx.h"
#include "EnemyManager.h"

#include <unordered_set>

#include "Actor\Enemy\Enemy.h"
#include "Actor\Enemy\EnemyMetaAi\EnemyMetaAi.h"

#include "Battle\BattleAreaManager.h"
#include "Battle\BattleManager.h"

#include "Actor\Enemy\EnemySystem.h"

#include "Random.h"

//インスタンス初期化
EnemyManager* EnemyManager::m_instance = nullptr;

void EnemyManager::InitEnemyManager()
{
	//戦闘開始をバトルマネージャーから通知されるように登録
	BattleManager::GetInstance()->RegisterBattleStartCallBack(
		[&](const BattleStartEventInfo& eventInfo)
		{
			EnemyGroupeBattleSet(*eventInfo.m_enemyGroupeInfo);
		}
	);

	m_enemyMetaAi = NewGO<EnemyMetaAi>(UpdateOrder::AI, "enemymetaai");
	m_enemyAiSystem = NewGO<EnemySystem>(UpdateOrder::AI, "enemy");
}

void EnemyManager::RequestSpawnEnemy(EnemyYakuzaType type, const Vector3& spawnPoint)
{	
	EnemyPair pair;

	auto newEnemy = m_enemyFactory.CreateEnemy(type);

	pair.m_enemy = newEnemy;

	pair.m_enemyAi = m_enemyAiFactory.GetInstance().Create(type, &newEnemy->GetYakuzaStateMachine());

	newEnemy->SetPosition(spawnPoint);

	m_enemyPairList.push_back(std::move(pair));
}

void EnemyManager::RequestSpawnEnemyGroup(int spawnNum, const Vector3& spawnPoint,bool inSpYakuza)
{
	EnemyGroup newGroup;

	for (int i = 0; i < spawnNum; i++)
	{
		EnemyPair pair;

		EnemyYakuzaType type;
		int  randomType = -1;

		if (inSpYakuza &&
			i >= spawnNum - 1)
		{
			//タイプをランダム選定
			randomType = EnemyYakuzaType::en_toughYakuza;
			//Random::Range(EnemyYakuzaType::en_normalYakuza)
			type = static_cast<EnemyYakuzaType>(randomType);
		}
		else
		{
			//タイプをランダム選定
			randomType = EnemyYakuzaType::en_normalYakuza;
			//Random::Range(EnemyYakuzaType::en_normalYakuza)
			type = static_cast<EnemyYakuzaType>(randomType);
		}

		auto newEnemy = m_enemyFactory.CreateEnemy(type);

		pair.m_enemy = newEnemy;

		pair.m_enemyAi = m_enemyAiFactory.GetInstance().Create(type, &newEnemy->GetYakuzaStateMachine());

		pair.m_enemyID = m_enemyIDCounter;

		//仮名なので注意
		pair.m_enemyName = EnemyTempNames[i];

		//スポーン位置をランダム選定
		Vector3	randomSpawnPoint = GetRandomPointInRadius(spawnPoint, 100.0f);
	
		newEnemy->SetPosition(randomSpawnPoint);

		m_enemyPairList.push_back(std::move(pair));

		newGroup.m_enemyID.push_back(m_enemyIDCounter);

		m_enemyIDCounter++;
	}

	//戦闘エリア生成
	int areaId = BattleAreaManager::GetInstance()->CreateArea(spawnPoint, 400.0f);

	newGroup.m_battleAreaId = areaId;

	newGroup.m_groupeId = m_enemyGroupIDCounter++;

	m_enemyGroupList.push_back(std::move(newGroup));
}

void EnemyManager::RequestDeadEnemyProcces(const Enemy& deadEnemyAddress)
{
	//削除したエネミーのID
	int deleteEnemyId = -1;

	//エネミーペアリスト削除処理
	for (auto it = m_enemyPairList.begin();it != m_enemyPairList.end();)
	{
		//引数のアドレスと合うエネミーを検索
		if (it->m_enemy == &deadEnemyAddress)
		{
			deleteEnemyId = it->m_enemyID;

			it = m_enemyPairList.erase(it);
		}
		else
		{
			it++;
		}
	}

	//グループリストID削除処理
	for (auto it = m_enemyGroupList.begin();it != m_enemyGroupList.end();)
	{
		//グループ内IDを探す
		for (auto idList = it->m_enemyID.begin();idList != it->m_enemyID.end();)
		{
			//IDが同じだったら
			if (deleteEnemyId == *idList)
			{
				//削除
				idList = it->m_enemyID.erase(idList);
			}
			else
			{
				idList++;
			}
		}

		it++;
	}
}

void EnemyManager::RequestResetEnemysProcees()
{
	//まずエネミーがいないかを確かめる
	if (m_enemyPairList.empty() &&
		m_enemyGroupList.empty())
	{
		//どちらも空だったら
		//情報リストを空にする
		m_enemyInfoList.clear();
		//メタAIを削除
		DeleteGO(m_enemyMetaAi);
		//AIシステムを削除
		DeleteGO(m_enemyAiSystem);
		//カウンター類を0に
		m_enemyIDCounter = 0;
		m_enemyGroupIDCounter = 0;

		return;
	}

	//もしエネミーが残っている場合は削除処理を行う
	m_enemyGroupList.clear();
	m_enemyInfoList.clear();

	for (auto pairIt = m_enemyPairList.begin();pairIt != m_enemyPairList.end();)
	{
		if (pairIt->m_enemy)
		{
			DeleteGO(pairIt->m_enemy);
		}

		pairIt = m_enemyPairList.erase(pairIt);
	}
	//メタAIを削除
	DeleteGO(m_enemyMetaAi);
	//AIシステムを削除
	DeleteGO(m_enemyAiSystem);
	//カウンター類を0に
	m_enemyIDCounter = 0;
	m_enemyGroupIDCounter = 0;
}

void EnemyManager::Update()
{
	//エネミーグループリスト更新処理
	UpdateEnemyGroupe();
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

void EnemyManager::UpdateEnemyGroupe()
{
	for (auto it = m_enemyGroupList.begin(); it != m_enemyGroupList.end();)
	{
		//グループの削除フラグが経っていたら削除
		if (it->m_isDelete)
		{
			it = m_enemyGroupList.erase(it);
		}
		else
		{
			it++;
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

	//グループのIDがこのままでは要素数しか代入できないので修正予定
	for (auto& groupPtr : enemyGroupList)
	{
		int groupId = groupPtr.m_groupeId;
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
			newGroup.m_battleAreaId = groupPtr.m_battleAreaId;
			m_enemyInfoList.push_back(newGroup);
			existGroup = &m_enemyInfoList.back();
		}
		//新規でないならグループ情報を更新
		else
		{
			existGroup->m_inBattle = groupPtr.isInBattle;
		}

		//グループ内部の更新
		existGroup->m_enemyAiInfoList.clear();

		//EnemyIdごとにEnemyMemberInfowo作成
		for (auto& id : groupPtr.m_enemyID)
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

	m_targetView.m_isTargetAttacking = m_targetCharacter->GetYakuzaStateMachine().GetIsAttack();

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

void EnemyManager::EnemyGroupeBattleSet(EnemyInfoGroupe& battleInEnemyGroupe)
{
	SetEnemyGroupeInBattle(battleInEnemyGroupe.m_groupId, true);
}