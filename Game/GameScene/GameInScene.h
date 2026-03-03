#pragma once
#include "GameScene\IGameSceneState.h"

#include "Bgm\BgmManager.h"

class Player;
class PlayerController;
class PoseMenu;
class RecoveryItem3DModel;
class VolumeAdjustment;
class ProtoStage;

class EnemysHpGauge;

class Inventory;

class MiniMap;

enum GameState
{
    //ゲームロード中
    en_gameLoad,
    //ゲーム更新
    en_gameUpdate,
    //ゲームオーバー処理に移行
    en_gameOver,
    //ゲームクリア処理に移行
    en_gameClear,
    //タイトルに戻る
    en_returnToTitle
};

enum GameInUpdateState
{
    en_firstEnemys,
    en_firstBoss
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
    /// <summary>
    /// ゲームイン更新
    /// </summary>
    bool GameInStateUpdate();
    /// <summary>
    /// ゲームオブジェクト削除
    /// </summary>
    void DeleteGameObjects();
private:

    int GetEnemyTypeBattleBgm(EnemyYakuzaType enemyType);
    ///プレイヤーのポインタ
    Player* m_player = nullptr;
    ///プレイヤーコントローラーのポインタ
    PlayerController* m_playerController = nullptr;

    PoseMenu* m_poseMenu = nullptr;
    RecoveryItem3DModel* m_recoveryItem3DModel = nullptr;
    VolumeAdjustment* m_volumeAdjustment = nullptr;

    Vector3 m_bossAreaCenter = Vector3::Zero;

    Inventory* m_inventory = nullptr;
    //プロトステージ
    ProtoStage* m_protoStage = nullptr;
    //エネミーのHP
    EnemysHpGauge* m_enemysHpGauge = nullptr;
    //ゲーム内ステート
    GameState m_gameState = GameState::en_gameLoad;
	//ゲーム内更新ステート
	GameInUpdateState m_gameInUpdateState = GameInUpdateState::en_firstEnemys;
    //ミニマップ
    MiniMap* m_miniMap = nullptr;
    //スカイキューブ
    SkyCube* m_skyCube = nullptr;
    ///レベル
    LevelRender m_level;
};

