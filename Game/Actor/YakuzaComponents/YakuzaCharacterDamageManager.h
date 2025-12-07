#pragma once
#include "Actor\YakuzaComponents\YakuzaCharacter.h"

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
	
	//引数のコリジョンを持っている敵を検索して引数分のダメージを与える
	void SendEnemyDamage(CollisionObject* collision, float sendDamage);
	//引数のコリジョンを持っているその他のYakuzaを検索して引数分のダメージを与える
	void SendOtherYakuzaDamage(CollisionObject* collision, float sendDamage);

	void SetDamageList(YakuzaCharacter* setYakuza)
	{
		m_sendDamageOtherYakuzaList.push_back(setYakuza);
	}

	//ダメージを与えるその他のYakuzaのリスト
	std::vector<YakuzaCharacter*> m_sendDamageOtherYakuzaList;
};

