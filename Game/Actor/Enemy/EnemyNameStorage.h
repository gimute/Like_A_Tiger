#pragma once

class EnemyNameStorage
{
public:

	void Initialize(const std::vector<std::string>& names)
	{
		m_masterNames = names;
		Reset();
	}

	//敵名をランダムに取得
	std::string GetName();

private:

	void Reset()
	{
		m_availableNames = m_masterNames;
	}

	//初期化用の名前リスト
	std::vector<std::string> m_masterNames;
	//実際に使用する名前リスト
	std::vector <std::string> m_availableNames;
};

