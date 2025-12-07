#pragma once

class YakuzaCharacterDamageManager
{
private:
	//シングルトーンパターンを採用するため、コンストラクタをprivateにする
	//インスタンス
	static YakuzaCharacterDamageManager* m_instance;
	//コンストラクタ
	YakuzaCharacterDamageManager();
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
};

