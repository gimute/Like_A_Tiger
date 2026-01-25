#pragma once
#include "Actor\Player\Player.h"
#include "Actor\Enemy\Enemy.h"

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

	//プレイヤーに敵を掴んだ事を伝える
	YakuzaCharacter* SendPlayerGrabEnemyYakuza(YakuzaCharacter* grabYakuza);

	//敵にプレイヤーを掴んだ事を伝える
	YakuzaCharacter* SendEnemyGrabPlayerYakuza(YakuzaCharacter* grabYakuza);

	//掴み時に互いの位置を更新する処理
	bool UpdateBothYakuzaGrabProcess(YakuzaCharacter* grabingYakuza, YakuzaCharacter* grabBedYakuza);

	//掴み時に相手に与えるダメージ等の処理(falseで通常攻撃、trueでフィニッシュ)
	void SendGrabingYakuzaDamage(YakuzaCharacter* grabingYakuza, int isAttackType);

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

