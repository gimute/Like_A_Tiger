#pragma once
#include "GameScene\IGameSceneState.h"

enum GameOverState
{
	en_GameOverStandby,
	en_GameOverLoadIn,
	en_GameOverWaitButton,
	en_GameOverProceesEnd
};

class GameOverScene : 
	public IGameSceneState
{
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameOverScene() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameOverScene() = default;
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
	//ゲームオーバーのステート
	GameOverState m_gameOverState = GameOverState::en_GameOverStandby;
	//リザルトのスプライト

};

