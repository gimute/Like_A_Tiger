#pragma once

/*
* ParameterManager.h
* パラメーター管理
* こいつにキャラクターのステータスなど外部ファイルから読み込ませて保持させ、それを受け取って使う。
* パラメーターの識別をファイルパスで行っているので、パラメーター取得、解放などで事あるごとにパスを要求するが、
* LoadParameter関数以外で読み込みを行ったりはしない。
* シングルトンクラス。
*/

#include <iostream>
#include <fstream>

/////////////////////////////////////////////
// パラメーター
/////////////////////////////////////////////

//パラメーター基底構造体
struct IParameter {};

//プレイヤーのステータス
struct PlayerStatusParamater : public IParameter
{
	float maxHP;	//最大HP
};

//エネミーのステータス
struct EnemyStatusParamater : public IParameter
{
	float maxHP;	//最大HP
};

#undef appParameter

/// <summary>
/// パラメーター管理クラス
/// </summary>
class ParameterManager
{
private:
	using ParameterVector = std::vector<IParameter*>;
	using ParameterMap = std::map<std::string, ParameterVector>;

private:
	ParameterMap m_parameterMap;	//パラメータとIDのリスト

private:
	ParameterManager();
	~ParameterManager();

public:
	/// <summary>
	/// パラメーターファイルを読み込む
	/// どんなパラメーターがあるのかはマネージャーにはわからないので関数ポインタで読み込み処理を受け取る
	/// </summary>
	/// <typeparam name="T">パラメーターの種類</typeparam>
	/// <param name="path">ファイルパス</param>
	/// <param name="func">読み込み処理</param>
	template<typename T>
	void LoadParameter(const char* path, const std::function<void(const nlohmann::json& json, T& p)>& func)
	{
		//ファイルを開く
		std::ifstream file(path);
		if (!file.is_open())
		{
			return;
		}

		//jsonファイルとして読み込む？
		nlohmann::json jsonRoot;
		file >> jsonRoot;

		//読み込んだパラメーターを一時的に持つ受け皿
		ParameterVector tmpParameters;

		for (const auto& j : jsonRoot)
		{
			T* parameter = new T();
			//パラメータ読み込み処理
			func(j, *parameter);
			tmpParameters.push_back(static_cast<IParameter*>(parameter));
		}

		//パラメーターを登録
		m_parameterMap.emplace(path, tmpParameters);
	}

	/// <summary>
	/// パラメーター解放
	/// </summary>
	/// <param name="path">解放するパラメーターのファイルパス</param>
	void UnloadParameter(const char* path)
	{
		auto it = m_parameterMap.find(path);
		if (it != m_parameterMap.end())
		{
			auto& parameters = it->second;
			for (auto* p : parameters)
			{
				delete p;
			}
			m_parameterMap.erase(it);
		}
	}

	/// <summary>
	/// パラメーターの取得
	/// </summary>
	/// <typeparam name="T">取得するパラメーターの構造体</typeparam>
	/// <param name="path">取得するパラメーターのファイルパス</param>
	/// <param name="index">一つのファイルに複数のパラメーターを入れた場合は何番目かこれで指定する</param>
	/// <returns></returns>
	template <typename T>
	const T* GetParameter(const char* path, const int index = 0) const
	{
		const auto parameters = GetParameters<T>(path);
		if (parameters.size() == 0)
		{
			return nullptr;
		}
		if (parameters.size() <= index)
		{
			return nullptr;
		}

		return parameters[index];
	}

	/// <summary>
	/// 複数のパラメーターを取得する
	/// </summary>
	/// <typeparam name="T">取得するパラメーターの構造体</typeparam>
	/// <param name="path">取得するパラメーターのファイルパス</param>
	/// <returns></returns>
	template <typename T>
	const std::vector<T*> GetParameters(const char* path) const
	{
		std::vector<T*> parameters;

		auto it = m_parameterMap.find(path);
		if (it != m_parameterMap.end())
		{
			for (auto* parameter : it->second)
			{
				parameters.push_back(static_cast<T*>(parameter));
			}
		}
		return parameters;
	}

	/// <summary>
	/// パラメーターをラムダ式で回す
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="path"></param>
	/// <param name="func"></param>
	template<typename T>
	void ForEach(const char* path, std::function<void(const T&)> func) const
	{
		const std::vector<T*> parameters = GetParameters<T>(path);
		for (const T* parameter : parameters)
		{
			func(*parameter);
		}
	}


	/*
	* シングルトン用コード
	*/
private:
	static ParameterManager* m_instance;
public:

	/// <summary>
	/// インスタンス生成
	/// </summary>
	static void CreateInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new ParameterManager();
		}
	}

	/// <summary>
	/// インスタンスを取得
	/// </summary>
	/// <returns></returns>
	static ParameterManager& GetInstance()
	{
		return *m_instance;
	}

	static void DestroyInstance()
	{
		if (m_instance != nullptr)
		{
			delete m_instance;
			m_instance = nullptr;
		}
	}
};


