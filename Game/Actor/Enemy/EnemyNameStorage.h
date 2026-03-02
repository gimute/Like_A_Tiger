#pragma once

class EnemyNameStorage
{
private:
	//インスタンス
	static EnemyNameStorage* m_instance;
	//コンストラクタ
	EnemyNameStorage() = default;
	//コピー禁止
	EnemyNameStorage(const EnemyNameStorage&) = delete;
	//代入禁止にする
	EnemyNameStorage& operator=(const EnemyNameStorage) = delete;
public:
	//デストラクタ
	~EnemyNameStorage() = default;
	//インスタンス取得
	static EnemyNameStorage* GetInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new EnemyNameStorage();
		}
		return m_instance;
	}

	//初期化
	void Initialize(const std::vector<std::string>& names)
	{
		m_masterNames = names;
		Reset();
	}

	//敵名をランダムに取得
	std::string GetName();
private:

	//有効名ストレージが空になったときにリセットする
	void Reset()
	{
		m_availableNames = m_masterNames;
	}

	//初期化用の名前リスト
	std::vector<std::string> m_masterNames;
	//実際に使用する名前リスト
	std::vector <std::string> m_availableNames;
};

