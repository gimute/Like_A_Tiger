#pragma once
#include "Inventory/Types.h"


/**
 * ‚½‚Ô‚ñ1ŒÂ
 * ŠO‚©‚çì‚ç‚ê‚È‚¢‚æ‚¤‚É‚Æ‚PŒÂ‚¾‚¯‚É‚·‚éˆ—‚¾‚¯‚µ‚ÄAæ“¾‚Í‚³‚¹‚È‚¢‚æ‚¤‚É‚µ‚æ‚¤‚©‚È
 */
class Inventory
{
private:
	std::vector<std::unique_ptr<ItemInfo>> m_itemInfoList;

private:
	Inventory();
	~Inventory();


public:
	void ForEach(const std::function<void(const ItemInfo*)>& func)
	{
		for (auto& itemInfo : m_itemInfoList) {
			func(itemInfo.get());
		}
	}

private:
	static Inventory* m_instance;

public:
	static Inventory* Create()
	{
		if (m_instance == nullptr)
		{
			m_instance = new Inventory();
		}
		return m_instance;
	}
	static void Delete()
	{
		if (m_instance) {
			delete m_instance;
			m_instance = nullptr;
		}
	}
};


// ‚¢‚Â‚à‚Ícpp‚Å‚·‚é‚¯‚ÇA‚±‚±‚Å‘‚¢‚Ä‚İ‚½‚—
//Inventory* Inventory::m_instance = nullptr;