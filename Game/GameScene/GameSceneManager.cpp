#include "stdafx.h"
#include "GameSceneManager.h"

#include "IGameSceneState.h"

/*
	★シーンマネージャーの使い方★
	まずはこのGameSceneファイルにてIGameSceneState.hを継承したステートクラスを作成します。
	IGameSceneState.hにはステートに必要な純粋仮想関数が定義されているので、継承したクラスでオーバーライドして実装します。
	priveateにappState(クラス名)を記述することで、そのクラスのハッシュ値を取得できるようになります。これを記述しないとステートマップに登録できません。
	
	ステートクラスを作成したら、GameSceneManager.hのAddSceneState関数でステートマップに登録します。
	ステートマップに登録したら、ReqestInitSceneState関数で初期化したいステートを指定します。(初期はTitleSceneです。書き換えてデバック等に活用してください)
	あとはGame.cppのUpdate関数内でUpdateGameSceneState関数を呼び出すことで、ステートの更新が行われます。

	ステートの変更は、各ステートクラス内でReqestSceneState関数をオーバーライドして実装します。
	ReqestSceneState関数内で、引数のnextStateに変更したいステートのハッシュ値を代入してtrueを返すことで、ステートの変更が行われます。
	ステートの変更が行われると、現在のステートのExitScene関数が呼ばれ、次のステートのEnterScene関数が呼ばれます。
	ステートの変更が行われない場合は、UpdateScene関数が呼ばれ続けます。

	以上解説はCopilotさんでした、ＡＩすごいねこれ。
*/

//インスタンスの初期化
GameSceneManager* GameSceneManager::m_SceneManagerInstance = nullptr;

void GameSceneManager::InitGameSceneManager()
{

}

void GameSceneManager::CurrentStateEnter()
{
	m_currentState->EnterScene();
}

void GameSceneManager::UpdateGameSceneState()
{
	if (m_currentState)
	{
		//ハッシュ値、リクエストされたステート
		uint32_t reqestState = 0;
		if (m_currentState->ReqestSceneState(reqestState))
		{
			//ステートの終了を実行
			m_currentState->ExitScene();
			//ステートをハッシュマップから探して変更
			m_currentState = FindState(reqestState);
			//ステートの初期化処理を実行
			m_currentState->EnterScene();
		}
	}
	//ステートを更新
	m_currentState->UpdateScene();
}