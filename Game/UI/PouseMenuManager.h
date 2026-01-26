#pragma once
#include "CRC32.h"

/** 前方宣言 */
class PoseMenu;

#define appState(name) \
public:\
	static constexpr uint32_t ID(){ return Hash32(#name);}

/** ステートの基底インターフェース */
class IPouseMenuSceneState {
public:
	IPouseMenuSceneState() = default;
	virtual ~IPouseMenuSceneState() {}

public:
	virtual void EnterScene() = 0;
	virtual void UpdateScene() = 0;
	virtual void ExitScene() = 0;
	virtual bool RequestSceneState(uint32_t& nextState) = 0;

public:
	/** 操作対象のメニューUIの設定 */
	void SetOwner(PoseMenu* owner) { m_owner = owner; }

protected:
	PoseMenu* m_owner = nullptr;
};




/**************************************************/

/** 共通の親クラス */
class PouseMenuSceneBase : public IPouseMenuSceneState {
public:
	virtual ~PouseMenuSceneBase() = default;

	/** タブ切り替え、閉じる */
	bool CheckCommonTransition(uint32_t& nextState);

	/** 自分の左右のタブIDを返す */
	virtual uint32_t GetNextTabID() {
		return 0;
	}

	virtual uint32_t GetPrevTabID() {
		return 0;
	}
};




/**************************************************/


/** メニュー内のシーン */
class PouseMenuInSideScene : public PouseMenuSceneBase {
	appState(PouseMenuInSideScene);
public:
	PouseMenuInSideScene() = default;
	virtual ~PouseMenuInSideScene() = default;

public:
	void EnterScene() override;
	void UpdateScene() override;
	void ExitScene() override;
	bool RequestSceneState(uint32_t& nextState) override;

private:
	bool m_isAnimationDone = false;
};




/*************************************************/


/** メニュー外のシーン */
class PouseMenuOutSideScene : public PouseMenuSceneBase {
	appState(PouseMenuOutSideScene);
public:
	PouseMenuOutSideScene() = default;
	virtual ~PouseMenuOutSideScene() = default;

public:
	void EnterScene() override;
	void UpdateScene() override;
	void ExitScene() override;
	bool RequestSceneState(uint32_t& nextState) override;

private:
	bool m_isAnimationDone = false;
};




/*************************************************/


/** リストシーン */
class PouseMenuListScene : public PouseMenuSceneBase {
	appState(PouseMenuListScene);
public:
	PouseMenuListScene() = default;
	virtual ~PouseMenuListScene() = default;

public:
	void EnterScene() override;
	void UpdateScene() override;
	void ExitScene() override;
	bool RequestSceneState(uint32_t& nextState) override;

public:
	//  uint32_t GetNextTabID() override;
	//  uint32_t GetPrevTabID() override;
};




/*************************************************/


/** アイテムシーン */
class PouseMenuItemScene : public PouseMenuSceneBase {
	appState(PouseMenuItemScene);
public:
	PouseMenuItemScene() = default;
	virtual ~PouseMenuItemScene() = default;

public:
	void EnterScene() override;
	void UpdateScene() override;
	void ExitScene() override;
	bool RequestSceneState(uint32_t& nextState) override;

public:
	uint32_t GetNextTabID() override;
	uint32_t GetPrevTabID() override;
};




/*************************************************/


/** 操作説明シーン */
class PouseMenuManualControlScene : public PouseMenuSceneBase {
	appState(PouseMenuManualControlScene);
public:
	PouseMenuManualControlScene() = default;
	virtual ~PouseMenuManualControlScene() = default;

public:
	void EnterScene() override;
	void UpdateScene() override;
	void ExitScene() override;
	bool RequestSceneState(uint32_t& nextState) override;

public:
	uint32_t GetNextTabID() override;
	uint32_t GetPrevTabID() override;
};




/*************************************************/


/** 設定シーン */
class PouseMenuSettingScene : public PouseMenuSceneBase {
	appState(PouseMenuSettingScene);
public:
	PouseMenuSettingScene() = default;
	virtual ~PouseMenuSettingScene() = default;

public:
	void EnterScene() override;
	void UpdateScene() override;
	void ExitScene() override;
	bool RequestSceneState(uint32_t& nextState) override;

public:
	uint32_t GetNextTabID() override;
	uint32_t GetPrevTabID() override;
};




/*************************************************/


/**
 * TODO:ステートマップってなんぞや⇒
 *		アンオーダーマップってなんぞや⇒
 *		一旦、調べて、自分の言葉で説明して
 *		AIに確認とる
 */

/** マネージャー */
class PouseMenuSceneManager{
private:
	/** 名前定義 */

	/** ステートマップ */
	using StateMap = std::unordered_map<uint32_t, IPouseMenuSceneState*>;
	/** ステートペア */
	using StatePair = std::pair<uint32_t, IPouseMenuSceneState*>;
	
private:
	/** ステートマップ変数 */
	StateMap m_stateMap;
	/** 現在のステート変数 */
	IPouseMenuSceneState* m_currentState = nullptr;
	/** インスタンス */
	static PouseMenuSceneManager* m_sceneManagerInstance;
	/** メニュー本体を保持しておく変数 */
	PoseMenu* m_menuOwner = nullptr;

private:
	/** コンストラクタ */
	PouseMenuSceneManager() : m_currentState(nullptr)
	{
		m_stateMap.clear();
	}

	/** コピー禁止 */
	PouseMenuSceneManager(const PouseMenuSceneManager&) = delete;

	/** 代入禁止 */
	PouseMenuSceneManager& operator=(const PouseMenuSceneManager&) = delete;

public:
	/** デストラクタ */
	~PouseMenuSceneManager()
	{
		for (auto statePtr : m_stateMap)
		{
			delete statePtr.second;
			statePtr.second = nullptr;
		}
		m_stateMap.clear();
	}

	static PouseMenuSceneManager* GetSceneManagerInstance()
	{
		if (m_sceneManagerInstance == nullptr)
		{
			m_sceneManagerInstance = new PouseMenuSceneManager;
		}
		return m_sceneManagerInstance;
	}

public:
	/** ポーズメニューマネージャー初期化 */
	void InitPouseMenuSceneManager();

	/** ポーズメニューマネージャー更新 */
	bool UpdatePouseMenuSceneManager();

	/** ステートマップ登録 */
	template<typename T,typename...Args>
	inline void AddSceneState(Args&&... args)
	{
		m_stateMap.insert(StatePair(T::ID(), new T(std::forward<Args>(args)...)));
	}

	/** 外部からステートを変更 */
	template<typename T>
	inline void RequestInitSceneState()
	{
		//ステートを探して変更
		m_currentState = FindState(T::ID());

		/** 初期ステートにもUIを渡して開始 */
		if (m_currentState) {
			m_currentState->SetOwner(m_menuOwner);
			m_currentState->EnterScene();
		}
	}

	/** GameInSceneからメニュー本体を受け取るための関数 */
	void SetMenuOwner(PoseMenu* owner) {
		m_menuOwner = owner;
	}

private:

	inline IPouseMenuSceneState* FindState(uint32_t stateID)
	{
		const auto& it = m_stateMap.find(stateID);
		if (it == m_stateMap.end())
		{
			return nullptr;
		}
		return it->second;
	}
};

