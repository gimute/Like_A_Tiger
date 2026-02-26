#pragma once
#include "Actor\Character.h"
#include "Actor\YakuzaComponents\YakuzaType.h"

class Enemy;

struct EnemyFactoryReturnValue
{
	//生成したエネミーのポインタ
	Enemy* enemyPtr = nullptr;
	//生成した敵名
	std::string enemyName;
};

class EnemyFactory
{
public:

	//コンストラクタ
	EnemyFactory() = default;
	//デストラクタ
	~EnemyFactory() = default;

	//エネミー制作
	EnemyFactoryReturnValue CreateEnemy(EnemyYakuzaType type);
};

