#pragma once
#include "CRC32.h"

class IGameSceneState;

#define appState(name)  \
public:\
	static constexpr uint32_t ID(){ return Hash32(#name); }

class GameSceneStateMachine
{
protected:
	///名前定義
	/// <summary>
	/// ステートマップ
	/// </summary>
	using StateMap = std::unordered_map<uint32_t, IGameSceneState*>;
	/// <summary>
	/// ステートペア
	/// </summary>
	using StatePair = std::pair<uint32_t, IGameSceneState*>;
protected:
	/// <summary>
	/// ステートマップ変数
	/// </summary>
	StateMap m_stateMap;
	/// <summary>
	/// 現在のステート変数
	/// </summary>
	IGameSceneState* m_currentState = nullptr;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameSceneStateMachine() 
		: m_currentState(nullptr)
	{
		m_stateMap.clear();
	}
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~GameSceneStateMachine()
	{
		for (auto statePtr : m_stateMap)
		{
			delete statePtr.second;
			statePtr.second = nullptr;
		}
		m_stateMap.clear();
	}
};

class GameSceneManager : public GameSceneStateMachine
{
private:
	///シングルトーンパターンを採用するため、コンストラクタをprivateにする
	/// <summary>
	/// インスタンス
	/// </summary>
	static GameSceneManager* m_SceneManagerInstance;
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameSceneManager() = default;
	/// <summary>
	/// コピー禁止にする
	/// </summary>
	/// <param name=""></param>
	GameSceneManager(const GameSceneManager&) = delete;
	/// <summary>
	/// 代入禁止にする
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	GameSceneManager& operator=(const GameSceneManager&) = delete;
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameSceneManager() = default;
	/// <summary>
	/// ゲームシーンマネージャーインスタンス取得関数
	/// </summary>
	/// <returns></returns>
	static GameSceneManager* GetSceneManagerInstance()
	{
		if (m_SceneManagerInstance == nullptr)
		{
			m_SceneManagerInstance = new GameSceneManager();
		}

		return m_SceneManagerInstance;
	}
	/// <summary>
	/// ゲームシーンマネージャー初期化関数
	/// </summary>
	void InitGameSceneManager();
	/// <summary>
	/// ゲームシーンステートの更新
	/// </summary>
	void UpdateGameSceneState();
	/// <summary>
	/// ステートマップ登録関数
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <typeparam name="...Args"></typeparam>
	/// <param name="...args"></param>
	template<typename T,typename... Args>
	inline void AddSceneState(Args&&... args)
	{
		m_stateMap.insert(StatePair(T::ID(), new T(std::forward<Args>(args)...)));
	}
	/// <summary>
	/// 外部からステートを変更するための関数
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T>
	inline void ReqestSceneState()
	{
		m_currentState = FindState(T::ID());
	}
private:
	/// <summary>
	/// ステート探索関数
	/// </summary>
	/// <param name="stateID"></param>
	/// <returns></returns>
	inline IGameSceneState* FindState(uint32_t stateID)
	{
		const auto& it = m_stateMap.find(stateID);
		if (it == m_stateMap.end())
		{
			return nullptr;
		}
		return it->second;
	}
};
