#pragma once
//クラス名のハッシュ値化に必要
#include "CRC32.h"

#include "StateMachineComponents\IState.h"

#define appState(name)  \
public:\
	static constexpr uint32_t ID(){ return Hash32(#name); }

class IStateMachine
{
private:
	//命名定義
	//ステートマップ
	using StateMap = std::unordered_map<uint32_t, std::unique_ptr<IStateBase>>;
	//ステートマップ変数
	StateMap m_stateMap;
	//現在のステート
	IStateBase* m_currentState = nullptr;
public:
	//コンストラクタ
	IStateMachine() : m_currentState(nullptr)
	{

	}
	//デストラクタ
	virtual ~IStateMachine()
	{

	}
	//登録関数
	template<typename ClassName,typename... Args>
	inline void AddState(Args&&... args)
	{
		m_stateMap.emplace(
				ClassName::ID(),
				std::make_unique<ClassName>(std::forward<Args>(args)...)
		);
	}
	//外部からステートを探して変更する関数
	template<typename ClassName>
	inline void ReqestState()
	{
		//ステートを探して変更
		m_currentState = FindState(ClassName::ID());
	}
public:
	//純粋仮想関数、次のステートを取得する関数
	virtual IStateBase* GetNextState() = 0;
	//ステートマシンの更新関数
	inline void UpdateStateMachine()
	{
		if (m_currentState)
		{
			//ステートをハッシュ値で取得
			IStateBase* nextState = GetNextState();
			if (m_currentState != nextState)
			{
				//ステートアウト
				m_currentState->OnExit();
				//ステートアップデート
				m_currentState = nextState;
				//ステートイン
				m_currentState->OnEnter();
			}
			//ステート更新
			m_currentState->OnUpdate();
		}
	}
private:
	//ステート探索関数
	inline IStateBase* FindState(uint32_t stateID)
	{
		const auto& it = m_stateMap.find(stateID);
		if (it == m_stateMap.end())
		{
			//念のため
			return nullptr;
		}
		return it->second.get();
	}
protected:
	//クラス名でステート探索
	template<typename ClassName>
	inline IStateBase* FindClassNameState()
	{
		uint32_t id = ClassName::ID();
		const auto& it = m_stateMap.find(id);
		if (it == m_stateMap.end())
		{
			//念のため
			return nullptr;
		}
		return it->second.get();
	}
	//引数に設定したステートクラスが現在のステートと同じかどうかを判定
	template<typename ClassName>
	inline bool IsNowStateClassName()
	{
		uint32_t id = ClassName::ID();
		const auto& it = m_stateMap.find(id);

		if (it->second.get() == m_currentState)
		{
			return true;
		}
		return false;
	}
};

