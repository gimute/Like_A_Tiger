#pragma once
#include "Actor\Player\Player.h"
#include "Actor\Enemy\Enemy.h"

namespace
{
	constexpr float HIT_EFFECT_SCALE = 5.0f;		//攻撃ヒット時のエフェクトスケール
	constexpr float GUARD_EFFECT_SCALE = 10.0f;		//攻撃ガード時のエフェクトスケール
	constexpr float HIT_EFFECT_PLAY_POS_Y = 50.0f;	//エフェクト再生する高さ
}

class VolumeAdjustment;

class YakuzaCharacterDamageManager
{
private:
	//シングルトーンパターンを採用するため、コンストラクタをprivateにする
	//インスタンス
	static YakuzaCharacterDamageManager* m_instance;

	VolumeAdjustment* m_volumeAdjustment = nullptr;
	//コンストラクタ
	YakuzaCharacterDamageManager()
	{
		m_sweepTestCollider.Create(1.0f);
	}
	//コピー禁止
	YakuzaCharacterDamageManager(const YakuzaCharacterDamageManager&) = delete;
	//代入禁止にする
	YakuzaCharacterDamageManager& operator=(const YakuzaCharacterDamageManager) = delete;
public:
	//デストラクタ
	~YakuzaCharacterDamageManager() = default;
	//インスタンス取得
	static YakuzaCharacterDamageManager* GetInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new YakuzaCharacterDamageManager();
		}

		return m_instance;
	}
	
	//ポインタの敵にダメージを送る
	void SendEnemyYakuzaDamage(Enemy* sendEnemy, YakuzaDamageDatas sendDamage);

	//プレイヤーにダメージを送る
	void SendPlayerYakuzaDamage(YakuzaDamageDatas sendDamage,const Vector3& attackerPos);

	//プレイヤーに敵を掴んだ事を伝える
	YakuzaCharacter* SendPlayerGrabEnemyYakuza(YakuzaCharacter* grabYakuza);

	//敵にプレイヤーを掴んだ事を伝える
	YakuzaCharacter* SendEnemyGrabPlayerYakuza(YakuzaCharacter* grabYakuza);

	//掴み時に互いの位置を更新する処理
	bool UpdateBothYakuzaGrabProcess(YakuzaCharacter* grabingYakuza, YakuzaCharacter* grabBedYakuza);

	//投げの際に壁にめり込まないように位置調整を行う処理
	bool AdjustGrabBedYakuzaPositionOnThrow(
		YakuzaCharacter* grabingYakuza,
		YakuzaCharacter* grabBedYakuza,
		const Vector3& sweepDir,
		const Vector3& adjustDir,
		float sweepDistance
	);

	void UpdateGrabBedYakuzaThrownPosition(YakuzaCharacter* thrownYakuza, YakuzaCharacter* throwYakuza);

	//掴んでいる側が掴まれている側にデータを送る処理
	void SendGrabingToGrabBedYakuzaData(YakuzaCharacter* grabingYakuza, int isAttackType);
	//掴まれている側がダメージを受け取る処理
	void TakeGrabBedYakuzaDamage(YakuzaCharacter* self,int isAttackType);
	//掴まれている側が掴んでいる側にデータを送る処理
	void SendGrabBedToGrabingYakuzaData(YakuzaCharacter* grabBedYakuza,int isAttackType);

	//掴まれている側が掴みから脱出するまでの時間の処理
	bool UpdateGrabBedYakuzaEscapeTime(YakuzaCharacter* grabBedYakuza,bool isResistance);

	//プレイヤーからダメージを受け取る
	YakuzaDamageDatas GetPlayerYakuzaDamage();

	

	//void SetDamageList(YakuzaCharacter* setYakuza)
	//{
	//	m_sendDamageOtherYakuzaList.push_back(setYakuza);
	//}

	void SetSendDamagePlayer(Player* setPlayer)
	{
		m_playerPtr = setPlayer;
	}
private:

	//防御に成功しているかどうか
	bool IsDefenseSuccessful(
		const Vector3& defenderPos,
		const Vector3& defenderForward,
		const Vector3& attackerPos,
		float defenseAngleCos = 0.0f
	);

	////ダメージを与えるその他のYakuzaのリスト
	//std::vector<YakuzaCharacter*> m_sendDamageOtherYakuzaList;
	//プレイヤーのポインタ
	Player* m_playerPtr = nullptr;

	struct SweepResultWall : public btCollisionWorld::ConvexResultCallback
	{
		//接触したかどうか
		bool m_isHit = false;

		virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& covexResult, bool normalInWorldSpace)
		{
			//壁と接触したかどうかを判定
			if (covexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall)
			{
				return 0.0f;
			}

			m_isHit = true;
			return 0.0f;
		}
	};

	SphereCollider m_sweepTestCollider;
};

