#pragma once
#include "stdafx.h"

#include "StateMachineComponents\IState.h"
#include "Actor\YakuzaComponents\YakuzaAttackComboStateMachine.h"
#include "Actor\YakuzaComponents\YakuzaTypeSetFactory.h"

#include "Actor\YakuzaComponents\YakuzaType.h"
#include "Actor\YakuzaComponents\YakuzaAnimationState.h"

#include "Actor\Character.h"

#include "Sound\SoundId.h"

class YakuzaTypeSetFactory;

template<class ClassType>
class TypeSetAutoRegister
{
public:
	TypeSetAutoRegister(OthersYakuzaType type)
	{
		YakuzaTypeSetFactory::GetInstance().Register(type, []()
			{
				return std::make_unique<ClassType>();
			});
	}
	TypeSetAutoRegister(EnemyYakuzaType type)
	{
		YakuzaTypeSetFactory::GetInstance().Register(type, []()
		{
			return std::make_unique<ClassType>();
		});
	}
};

struct YakuzaDamageDatas
{
	//攻撃力
	float m_attackPow = 0.0f;
	//ノックバック力
	float m_knockBackPow = 0.0f;
	//この攻撃で流す音声ID
	int m_seId = -1;

	YakuzaDamageDatas() = default;

	YakuzaDamageDatas(
		float attackPow,
		float knockBackPow,
		int seId
	) 
		: m_attackPow(attackPow)
		, m_knockBackPow(knockBackPow)
		, m_seId(seId)
	{ }
};

struct YakuzaAttackSEDatas
{
	//風切り音
	int m_cuttingWindId = -1;

	YakuzaAttackSEDatas() = default;

	YakuzaAttackSEDatas(
		int cuttingWindId
	)
		: m_cuttingWindId(cuttingWindId)
	{ }
};

class IYakuzaTypeSet
{
public:
	//デストラクタ
	virtual ~IYakuzaTypeSet() = default;
	//ステート生成
	virtual void CreateActions(YakuzaAttackComboStateMachine* useAttackStateMachine) = 0;

protected:

	uint32_t m_firstAttackID = 0;

	uint32_t m_firstFinishBrowID = 0;

	const char* m_modelFilePath = nullptr;

	std::vector<Character::AnimationData> m_animationDataList;

	std::unordered_map<uint32_t,YakuzaDamageDatas> m_yakuzaDamageDataList;

	std::unordered_map<uint32_t, YakuzaAttackSEDatas> m_yakuzaAttackSEList;

	//攻撃ステートを追加＋攻撃時データを追加
	template<typename ClassName>
	inline void AddAttackState(YakuzaAttackComboStateMachine* useAttackStateMachine, YakuzaDamageDatas damageData,YakuzaAttackSEDatas seData)
	{
		useAttackStateMachine->AddState<ClassName>(useAttackStateMachine);

		m_yakuzaDamageDataList.emplace(
			ClassName::ID(),
			damageData
		);

		m_yakuzaAttackSEList.emplace(
			ClassName::ID(),
			seData
		);
	}

public:
	inline uint32_t GetFirstAttackID() { return m_firstAttackID; }

	inline uint32_t GetFirstFinishBrowID() { return m_firstFinishBrowID; }

	inline const char* GetModelFilePath() { return m_modelFilePath; }

	inline std::vector<Character::AnimationData>& GetAnimationDataList() { return m_animationDataList; }

	//攻撃力取得関数
	inline YakuzaDamageDatas GetAttackPower(YakuzaAttackComboStateMachine* useAttackStateMachine)
	{
		uint32_t nowStateId = useAttackStateMachine->GetNowCombo();

		YakuzaDamageDatas damageData = m_yakuzaDamageDataList.find(nowStateId)->second;

		return damageData;
	}

	//SEデータ取得関数
	inline YakuzaAttackSEDatas GetAttackSEDatas(uint32_t findAttackStateID)
	{
		return m_yakuzaAttackSEList.find(findAttackStateID)->second;
	}
};

