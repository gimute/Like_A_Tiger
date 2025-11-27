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
	//初期化するステートID
	uint32_t m_initStateHash = 0;
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
	//登録関数(個別登録)
	inline void AddState(uint32_t Id, IStateBase* statePtr)
	{
		m_stateMap.emplace(
			Id,
			statePtr
		);
	}
	//外部からステートを探して変更する関数
	template<typename ClassName>
	inline void InitStateMachineClassName()
	{
		//ステートを探して変更
		m_initStateHash = ClassName::ID();
	}
	inline void InitStateMachineUINT32T(uint32_t stateID)
	{
		//ステートを探して変更
		m_initStateHash = stateID;
	}
public:
	//純粋仮想関数、次のステートを取得する関数
	virtual IStateBase* GetNextState() = 0;
	//ステートマシンの更新関数
	inline void UpdateStateMachine()
	{
		IStateBase* nextState = nullptr;

		//ステートをハッシュ値で取得
		if (m_currentState)
		{
			nextState = GetNextState();
		}
		else
		{
			nextState = FindClassUINT32TState(m_initStateHash);
		}

		if (m_currentState != nextState)
		{
			//ステートアウト
			if (m_currentState)
			{
				m_currentState->OnExit();
			}
			//ステートアップデート
			m_currentState = nextState;
			//ステートイン
			m_currentState->OnEnter();
		}
		//ステート更新
		m_currentState->OnUpdate();
	}
protected:
	//UINT32Tでステート探索
	inline IStateBase* FindClassUINT32TState(uint32_t stateID)
	{
		const auto& it = m_stateMap.find(stateID);
		if (it == m_stateMap.end())
		{
			//念のため
			return nullptr;
		}
		return it->second.get();
	}
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

