#pragma once
#include "GameScene\IGameSceneState.h"

class Player;
class PlayerController;
class PoseMenu;

class GameInScene :
    public IGameSceneState
{
	appState(GameInScene);
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    GameInScene() = default;
    /// <summary>
    /// デストラクタ
    /// </summary>
    ~GameInScene() = default;
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
    ///プレイヤーのポインタ
    Player* m_player = nullptr;
    ///プレイヤーコントローラーのポインタ
    PlayerController* m_playerController = nullptr;

    PoseMenu* m_poseMenu = nullptr;
};

