#pragma once
#include "GameScene\IGameSceneState.h"

class Player;
class PlayerController;
class PoseMenu;

class Inventory;

enum GameState
{
    //ゲームロード中
    en_gameLoad,
    //最初の敵群
    en_firstEnemyGroupe,
    //ゲーム終了に以降
    en_gameEnd
};

class GameInScene :
    public IGameSceneState
{
	appState(GameInScene)
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
    /// <summary>
    /// ゲームステート更新
    /// </summary>
    void GameStateUpdate();
private:
    ///プレイヤーのポインタ
    Player* m_player = nullptr;
    ///プレイヤーコントローラーのポインタ
    PlayerController* m_playerController = nullptr;

    PoseMenu* m_poseMenu = nullptr;

    Inventory* m_inventory = nullptr;
    //ゲーム内ステート
    GameState m_gameState = GameState::en_gameLoad;
};

