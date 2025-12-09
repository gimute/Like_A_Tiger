#pragma once



// アイテムの種類
enum EnItemType
{
	enItemType_Onigiri,
	enItemType_Bento,
	enItemType_Sushi,
	enItemType_Max,
};


/** アイコン表示用情報 */
struct ItemIconInformation
{
	EnItemType m_type;
	int m_num;
	//
	ItemIconInformation()
		: m_type(EnItemType::enItemType_Max)
		, m_num(3)
	{
	}
};


/** アイテム情報 */
struct ItemInfo
{
	EnItemType m_type;
	int m_num;

	// HP回復量
	// AP回復量
	// 速度
	// お金
	//
	ItemInfo() {}
};

