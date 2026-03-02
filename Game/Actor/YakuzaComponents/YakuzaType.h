#pragma once
//乱数などを使用して選定等に利用するためエネミーとその他で分ける

enum OthersYakuzaType
{
	en_playerYakuza,
	othersNum
};

enum EnemyYakuzaType
{
	en_normalYakuza,
	en_fastYakuza,
	en_toughYakuza,
	en_bossLineNum,
	en_bossHirotaYakuza = en_bossLineNum,
	enemyNum
};
