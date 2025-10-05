#include "stdafx.h"
#include "Game.h"

#include "GameScene\GameSceneManager.h"

bool Game::Start()
{

	GameSceneManager::GetSceneManagerInstance()->InitGameSceneManager();

	return true;
}

void Game::Update()
{

}

void Game::Render(RenderContext& rc)
{

}
