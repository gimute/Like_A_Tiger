#pragma once
#include "Actor\YakuzaComponents\IYakuzaTypeSet.h"

#include "CRC32.h"

class VolumeAdjustment;
#define appState(name)	\
public:\
	static constexpr uint32_t ID() { return Hash32(#name); }

class YakuzaGenericAttackState : public IStateBase 
{
protected:
	YakuzaAttackComboStateMachine* m_owner = nullptr;
	VolumeAdjustment* m_volumeAdjustment = nullptr;

	uint32_t m_hasAttackStateHash = 0;
public:
	YakuzaGenericAttackState(
		AttackStateInitData initData
	)
		: m_owner(initData.hasOwner)
		, m_yakuzaCamp(initData.yakuzaCamp)
		, m_nextAttackHash(initData.nextAttackHash)
		, m_nextFinalBrowHash(initData.nextFinalBrowHash)
		, m_playAnimationNo(initData.playAnimationNo)
		, m_attackSpeed(initData.attackSpeed)
		, m_attackAnimSpeed(initData.attackAnimSpeed)
	{
	}
	//次の通常攻撃コンボハッシュ値
	uint32_t m_nextAttackHash = 0;
	//次のファイナルブロウ攻撃コンボハッシュ値
	uint32_t m_nextFinalBrowHash = 0;
	//次に遷移するコンボハッシュ値
	uint32_t m_nextComboHash = 0;
	//再生するアニメーションナンバー
	int m_playAnimationNo = -1;
	//この攻撃ステートをどちらの陣営が使っているか
	YakuzaCamp m_yakuzaCamp = YakuzaCamp::en_campNone;
	//攻撃速度
	float m_attackSpeed = 0.0f;
	//攻撃モーション速度
	float m_attackAnimSpeed = 0.0f;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};

