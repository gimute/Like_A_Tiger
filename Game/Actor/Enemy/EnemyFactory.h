#pragma once
#include "Actor\Character.h"

#include "Actor\Enemy\EnemyType.h"

class EnemyFactory
{
public:

	//コンストラクタ
	EnemyFactory() = default;
	//デストラクタ
	~EnemyFactory() = default;

	//エネミー制作
	Character* CreateEnemy(EnemyType type);
};

