#pragma once
#include "Actor\Character.h"
#include "Actor\YakuzaComponents\YakuzaType.h"

class Enemy;

class EnemyFactory
{
public:

	//コンストラクタ
	EnemyFactory() = default;
	//デストラクタ
	~EnemyFactory() = default;

	//エネミー制作
	Enemy* CreateEnemy(EnemyYakuzaType type);
};

