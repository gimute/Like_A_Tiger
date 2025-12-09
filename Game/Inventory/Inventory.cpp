#include "stdafx.h"
#include "Inventory.h"


Inventory* Inventory::m_instance = nullptr;
Inventory::Inventory()
{
	// @todo for テスト用
	// 本来は別途情報をもらう感じになるはず
	
	// おにぎり
	{
		auto onigiriInfo = std::make_unique<ItemInfo>();
		onigiriInfo->m_type = enItemType_Onigiri;
		onigiriInfo->m_num = 3;

		m_itemInfoList.push_back(std::move(onigiriInfo));
	}

	// お弁当
	{
		auto bentoInfo = std::make_unique<ItemInfo>();
		bentoInfo->m_type = enItemType_Bento;
		bentoInfo->m_num = 4;

		m_itemInfoList.push_back(std::move(bentoInfo));
	}

	// お寿司
	{
		auto sushiInfo = std::make_unique<ItemInfo>();
		sushiInfo->m_type = enItemType_Sushi;
		sushiInfo->m_num = 2;

		m_itemInfoList.push_back(std::move(sushiInfo));
	}
}

Inventory::~Inventory()
{
	m_itemInfoList.clear();
}