#pragma once
#include "GameScene\IGameSceneState.h"
#include "GameScene\Result\Result.h"

enum ResultState
{
	en_Standby,
	en_LoadIn,
	en_WaitButtonTrigger,
	en_ProceesEnd
};

class GameResultScene :
	public IGameSceneState
{
	appState(GameResultScene)
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameResultScene() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameResultScene() = default;
	/// <summary>
	/// 純粋仮想関数、ステートに入るときに呼ばれる
	/// </summary>
	void EnterScene() override;
	/// <summary>
	/// 純粋仮想関数、ステートから出るときに呼ばれる
	/// </summary>
	void UpdateScene() override;
	/// <summary>
	/// 純粋仮想関数、ステートから出るときに呼ばれる
	/// </summary>
	void ExitScene() override;
	/// <summary>
	/// 純粋仮想関数、ステート変更要求
	/// </summary>
	/// <param name="nextState"></param>
	/// <returns></returns>
	bool ReqestSceneState(uint32_t& nextState) override;
private:
	//リザルトのプロセス
	ResultState m_resultState = ResultState::en_Standby;
	//リザルトのスプライト
	Result* m_resultSprite = nullptr;
	//次のシーンに進んでいいか
	bool m_isGoNextScene = false;
};

