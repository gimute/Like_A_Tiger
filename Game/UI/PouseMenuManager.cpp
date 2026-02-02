#include "stdafx.h"
#include "PouseMenuManager.h"
#include "PoseMenu.h"


bool PouseMenuSceneBase::CheckCommonTransition(uint32_t& nextState)
{
	/** 設定系UIがアクティブな場合はメニュー閉じを無効化 */
	if (m_owner->IsSettingInSelectActive()) {
		return false;
	}

	/** スタートボタンで閉じる */
	if (g_pad[0]->IsTrigger(enButtonStart)) {
		if (m_owner) {
			m_owner->Close();
		}
		nextState = PouseMenuOutSideScene::ID();
		return true;
	}
	return false;
}




/**************************************************/


void PouseMenuInSideScene::EnterScene()
{
	m_isAnimationDone = false;
	//if (m_owner) {
	//	m_owner->SetAlpha(0.0f);
	//}
}

void PouseMenuInSideScene::UpdateScene()
{
	//  if (!m_owner) return;
	//  
	//  /** アルファ値を徐々に上げる */
	//  float alpha = m_owner->GetAlpha().w;
	//  alpha += 5.0f * g_gameTime->GetFrameDeltaTime();
	//  
	//  if (alpha >= 1.0f) {
	//  	alpha = 1.0f;
	  	m_isAnimationDone = true;
	//  }
	//  m_owner->SetAlpha(alpha);
}

void PouseMenuInSideScene::ExitScene()
{
}

bool PouseMenuInSideScene::RequestSceneState(uint32_t& nextState)
{
	/** アニメーションが終わったら、アイテム描画へ */
	if (m_isAnimationDone) {
		nextState = PouseMenuListScene::ID();
		return true;
	}
	return false;
}




/**************************************************/


void PouseMenuOutSideScene::EnterScene()
{
	m_isAnimationDone = false;
}

void PouseMenuOutSideScene::UpdateScene()
{
	//if (!m_owner) return;
	//
	///** アルファ値を徐々に下げる */
	//float alpha = m_owner->GetAlpha().w;
	//alpha -= 5.0f * g_gameTime->GetFrameDeltaTime();
	//
	//if (alpha <= 0.0f) {
	//	alpha = 0.0f;
		m_isAnimationDone = true;
	//}
	//
	//m_owner->SetAlpha(alpha);
}

void PouseMenuOutSideScene::ExitScene()
{
}

bool PouseMenuOutSideScene::RequestSceneState(uint32_t& nextState)
{
	if (m_isAnimationDone) {
		nextState = 9999;
		return true;
	}

	return false;
}




/**************************************************/


void PouseMenuListScene::EnterScene()
{
	if (m_owner) {
		// 右側のカーソル操作を禁止
		m_owner->SetEnableContentCursol(false);
	}
}

void PouseMenuListScene::UpdateScene()
{
	if (!m_owner) return;

	/** SettingInSelectがアクティブな場合のみリスト操作を無効化 */
	if (m_owner->IsSettingInSelectActive()) {
		return;
	}

	// カーソル移動はCursolPosePanelが担当するため、ここでは処理しない
	// CursolPosePanel::Update()でm_owner->SetCurrentListIndex()が呼ばれる
}

void PouseMenuListScene::ExitScene()
{
}

bool PouseMenuListScene::RequestSceneState(uint32_t& nextState)
{
	/** メニュー閉じる */
	if (CheckCommonTransition(nextState)) {
		return true;
	}

	/** アニメーションが終わったら、アイテム描画へ */
	//  if (g_pad[0]->IsTrigger(enButtonB)) {
	//  	int currentIndex = m_owner->GetCurrentListIndex();
	//  
	//  	switch (currentIndex) {
	//  	case 0: nextState = PouseMenuItemScene::ID();
	//  		return true;
	//  	case 1: nextState = PouseMenuManualControlScene::ID();
	//  		return true;
	//  	case 2: nextState = PouseMenuSettingScene::ID();
	//  		return true;
	//  	}
	//  }
	return false;
}

//  uint32_t PouseMenuListScene::GetNextTabID()
//  {
//  	return 0;
//  }
//  
//  uint32_t PouseMenuListScene::GetPrevTabID()
//  {
//  	return 0;
//  }




/**************************************************/

void PouseMenuItemScene::EnterScene()
{
	if (m_owner) {
		/** アイテムパネル(0番目)を表示 */
		m_owner->SetCurrentListIndex(0);
		/** カーソル操作を解禁 */
		//-m_owner->SetEnableContentCursol(true);
	}
}

void PouseMenuItemScene::UpdateScene()
{
}

void PouseMenuItemScene::ExitScene()
{
}

bool PouseMenuItemScene::RequestSceneState(uint32_t& nextState)
{
	/** タブ切り替え・閉じる */
	if (CheckCommonTransition(nextState)) {
		return true;
	}

	//  /** Bボタンでリストシーンに戻る */
	//  if (g_pad[0]->IsTrigger(enButtonB)) {
	//  	nextState = PouseMenuListScene::ID();
	//  	return true;
	//  }

	return false;
}

uint32_t PouseMenuItemScene::GetNextTabID()
{
	return 0;
	//return PouseMenuManualControlScene::ID();
}

uint32_t PouseMenuItemScene::GetPrevTabID()
{
	return 0;
	//return PouseMenuSettingScene::ID();
}



/**************************************************/


void PouseMenuManualControlScene::EnterScene()
{
	if (m_owner) {
		/** アイテムパネル(1番目)を表示 */
		m_owner->SetCurrentListIndex(1);
	}
}

void PouseMenuManualControlScene::UpdateScene()
{
}

void PouseMenuManualControlScene::ExitScene()
{
}

bool PouseMenuManualControlScene::RequestSceneState(uint32_t& nextState)
{
	/** タブ切り替え・閉じる */
	if (CheckCommonTransition(nextState)) {
		return true;
	}

	//  /** Bボタンでリストシーンに戻る */
	//  if (g_pad[0]->IsTrigger(enButtonB)) {
	//  	nextState = PouseMenuListScene::ID();
	//  	return true;
	//  }

	return false;
}

uint32_t PouseMenuManualControlScene::GetNextTabID()
{
	return PouseMenuSettingScene::ID();
}

uint32_t PouseMenuManualControlScene::GetPrevTabID()
{
	return PouseMenuItemScene::ID();
}




/**************************************************/


void PouseMenuSettingScene::EnterScene()
{
	if (m_owner) {
		/** アイテムパネル(2番目)を表示 */
		m_owner->SetCurrentListIndex(2);
	}
}

void PouseMenuSettingScene::UpdateScene()
{
}

void PouseMenuSettingScene::ExitScene()
{
}

bool PouseMenuSettingScene::RequestSceneState(uint32_t& nextState)
{
	/** タブ切り替え・閉じる */
	if (CheckCommonTransition(nextState)) {
		return true;
	}

	//  if (g_pad[0]->IsTrigger(enButtonB)) {
	//  	nextState = PouseMenuListScene::ID();
	//  	return true;
	//  }

	return false;
}

uint32_t PouseMenuSettingScene::GetNextTabID()
{
	return PouseMenuItemScene::ID();
}

uint32_t PouseMenuSettingScene::GetPrevTabID()
{
	return PouseMenuManualControlScene::ID();
}




/**************************************************/

PouseMenuSceneManager* PouseMenuSceneManager::m_sceneManagerInstance = nullptr;

void PouseMenuSceneManager::InitPouseMenuSceneManager()
{
	m_currentState = nullptr;
	//m_menuOwner = nullptr;
	/** 念のため、既存のステートがあれば削除してクリア */
	for (auto statePtr : m_stateMap)
	{
		delete statePtr.second;
		statePtr.second = nullptr;
	}
	m_stateMap.clear();

	/** 各ステートの登録 */
	AddSceneState<PouseMenuInSideScene>();
	AddSceneState<PouseMenuOutSideScene>();
	AddSceneState<PouseMenuListScene>();
	AddSceneState<PouseMenuItemScene>();
	AddSceneState<PouseMenuManualControlScene>();
	AddSceneState<PouseMenuSettingScene>();
}

bool PouseMenuSceneManager::UpdatePouseMenuSceneManager()
{
	/** メニューが閉じている状態でスタートボタンを押したら開く */
	if (m_currentState == nullptr)
	{
		if (g_pad[0]->IsTrigger(enButtonStart)) {
			if (m_menuOwner) {
				m_menuOwner->Open();

				/** 初期ステートに遷移 */
				m_currentState = FindState(PouseMenuInSideScene::ID());
				if (m_currentState) {
					m_currentState->SetOwner(m_menuOwner);
					m_currentState->EnterScene();
				}
				m_isActive = true;
				return true;
			}
		}
		m_isActive = false;
		return false;
	}
	/** 確認UIが表示中の場合は、ステート遷移・更新を停止 */
	if (m_menuOwner && m_menuOwner->IsInSelectActive()) {
		m_isActive = true;
		return true;
	}

	// ハッシュ値、リクエストされたステート
	uint32_t requestState = 0;
	if (m_currentState->RequestSceneState(requestState))
	{
		/** 9999ならメニュー終了合図 */
		if (requestState == 9999) {
			//ステートの終了を実行
			m_currentState->ExitScene();
			m_currentState = nullptr;
			//m_menuOwner = nullptr;
			m_isActive = false;

			return false;
		}

		//ステートの終了を実行
		m_currentState->ExitScene();
		//ステートのハッシュマップから探して変更
		m_currentState = FindState(requestState);

		if (m_currentState) {
			m_currentState->SetOwner(m_menuOwner);
			//ステートの初期化処理を実行
			m_currentState->EnterScene();
		}
	}
	//ステートを更新
	if (m_currentState) {
		m_currentState->UpdateScene();
	}
	m_isActive = true;
	return true;
}

bool PouseMenuSceneManager::IsRequestReturnToTitle() const
{
	if (m_menuOwner) {
		return m_menuOwner->IsRequestReturnToTitle();
	}
	return false;
}