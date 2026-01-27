#pragma once



// アイテムの種類
enum EnItemType
{
	enItemType_Empty,		//空っぽ
	enItemType_Onigiri,		//おにぎり
	enItemType_Bento,		//お弁当
	enItemType_Sushi,		//お寿司
	enItemType_Max,
};


/** アイコン表示用情報 */
struct ItemIconInformation
{
	EnItemType m_type;
	//
	ItemIconInformation()
		: m_type(EnItemType::enItemType_Max)
	{
	}
};


/** アイテム情報 */
struct ItemInfo
{
	EnItemType m_type;

	// HP回復量
	// AP回復量
	// 速度
	// お金
	//
	ItemInfo() {}
};

