#pragma once

#include "GameScene\IGameSceneState.h"

class GameTitleScene :
    public IGameSceneState
{
    public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    GameTitleScene();
    /// <summary>
    /// デストラクタ
    /// </summary>
    ~GameTitleScene() override;
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
	bool ReqestSceneState(uint32_t nextState) override;
};

