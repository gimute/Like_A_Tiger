#pragma once
#include "Actor\Player\Player.h"
#include "Actor\Enemy\Enemy.h"

namespace
{
	constexpr float HIT_EFFECT_SCALE = 5.0f;		//攻撃ヒット時のエフェクトスケール
	constexpr float GUARD_EFFECT_SCALE = 10.0f;		//攻撃ガード時のエフェクトスケール
	constexpr float HIT_EFFECT_PLAY_POS_Y = 50.0f;	//エフェクト再生する高さ
}

class YakuzaCharacterDamageManager
{
private:
	//シングルトーンパターンを採用するため、コンストラクタをprivateにする
	//インスタンス
	static YakuzaCharacterDamageManager* m_instance;
	//コンストラクタ
	YakuzaCharacterDamageManager() = default;
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

};

