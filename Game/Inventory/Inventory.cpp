#include "stdafx.h"
#include "Inventory.h"
#include "SaveManager.h"


Inventory* Inventory::m_instance = nullptr;

Inventory* Inventory::Create() 
{
	if (m_instance == nullptr) {
		m_instance = new Inventory();
	}
	return m_instance;
}

void Inventory::Delete() {
	if (m_instance) {
		delete m_instance;
		m_instance = nullptr;
	}
}

Inventory::Inventory()
{
	// @todo for テスト用
	// 本来は別途情報をもらう感じになるはず
	
	// 初期化: 全てのスロットを空で埋めておく
	for (int i = 0; i < MAX_INVENTORY_SLOTS; ++i) {
		auto emptyInfo = std::make_unique<ItemInfo>();
		emptyInfo->m_type = enItemType_Empty;
		m_itemInfoList.push_back(std::move(emptyInfo));
	}

	// テストデータの追加
	AddItem(enItemType_Sushi);
	AddItem(enItemType_Bento);


	//  //1208/12:29
	//  // 空っぽにしてもあれじゃね....
	//  // 
	//  // おにぎり
	//  {
	//  	auto onigiriInfo = std::make_unique<ItemInfo>();
	//  	onigiriInfo->m_type = enItemType_Onigiri;
	//  
	//  	m_itemInfoList.push_back(std::move(onigiriInfo));
	//  }
	//  
	//  // お弁当
	//  {
	//  	auto bentoInfo = std::make_unique<ItemInfo>();
	//  	bentoInfo->m_type = enItemType_Bento;
	//  
	//  	m_itemInfoList.push_back(std::move(bentoInfo));
	//  }
	//  
	//  // お寿司
	//  {
	//  	auto sushiInfo = std::make_unique<ItemInfo>();
	//  	sushiInfo->m_type = enItemType_Sushi;
	//  
	//  	m_itemInfoList.push_back(std::move(sushiInfo));
	//  }
	//  {
	//  	auto sushiInfo = std::make_unique<ItemInfo>();
	//  	sushiInfo->m_type = enItemType_Sushi;
	//  
	//  	m_itemInfoList.push_back(std::move(sushiInfo));
	//  }
	//  {
	//  	auto sushiInfo = std::make_unique<ItemInfo>();
	//  	sushiInfo->m_type = enItemType_Sushi;
	//  
	//  	m_itemInfoList.push_back(std::move(sushiInfo));
	//  }

}

Inventory::~Inventory()
{
	m_itemInfoList.clear();
}

int Inventory::AddItem(EnItemType type) {
	// 先頭から空いている場所を探す
	for (int i = 0; i < MAX_INVENTORY_SLOTS; ++i) 
	{
		if (i >= m_itemInfoList.size()) break;

		if (m_itemInfoList[i]->m_type == enItemType_Empty) {
			// 空き場所にセット
			m_itemInfoList[i]->m_type = type;

			// 変更があったら即セーブ
			SaveManager::GetInstance().Save();
			return i;
		}
	}
	// 空きがなかった
	return -1;
}

void Inventory::RemoveItem(int index) {
	// 範囲チェック
	if (index < 0 || index >= MAX_INVENTORY_SLOTS) return;

	// データを削除して詰めるのではなく、空タイプで上書き
	m_itemInfoList[index]->m_type = enItemType_Empty;

	//変更があったら即セーブ
	SaveManager::GetInstance().Save();
}

ItemInfo* Inventory::GetItemInfo(int index) {
	if (index < 0 || index >= (int)m_itemInfoList.size()) return nullptr;
	return m_itemInfoList[index].get();
}

// セーブ機能用: 現在の全アイテムの種類をリストにして返す
std::vector<int> Inventory::GetItemInfoList() const
{
	std::vector<int> list;
	for (const auto& item : m_itemInfoList) {
		list.push_back(static_cast<int>(item->m_type));
	}
	return list;
}

// ロード機能用: 受け取った種類リストでインベントリを復元
void Inventory::LoadItemList(const std::vector<int>& types)
{
	for (int i = 0; i < MAX_INVENTORY_SLOTS; ++i) 
	{
		if (i < (int)types.size()) {
			//ロードデータにある情報をセット
			m_itemInfoList[i]->m_type = static_cast<EnItemType>(types[i]);
		}
		else {
			//ロードデータが足りない部分(拡張した場合など)は空にしておく
			m_itemInfoList[i]->m_type = enItemType_Empty;
		}
	}
}


int Inventory::GetItemCount(EnItemType type) const
{
	int count = 0;
	for (const auto& item : m_itemInfoList) 
	{
		if (item->m_type == type) {
			count++;
		}
	}

	return count;
}

void Inventory::Update() {
	//デバッグ用: 右ボタン(6キー)でアイテム追加
	if (g_pad[0]->IsPress(enButtonLB2)) {
		if (g_pad[0]->IsTrigger(enButtonA)) {
			AddItem(enItemType_Onigiri);
		}

		//RB1で弁当追加
		if (g_pad[0]->IsTrigger(enButtonX)) {
			AddItem(enItemType_Bento);
		}

		//寿司追加
		if (g_pad[0]->IsTrigger(enButtonY)) {
			AddItem(enItemType_Sushi);
		}
	}

	

	//  /** スロットに各アイテムを追加できているかのテスト */
	//  if (g_pad[0]->IsTrigger(enButtonRight)) {
	//  	//6
	//  	auto onigiriInfo = std::make_unique<ItemInfo>();
	//  	onigiriInfo->m_type = enItemType_Onigiri;
	//  
	//  	m_itemInfoList.push_back(std::move(onigiriInfo));
	//  }
	//  
	//  if (g_pad[0]->IsTrigger(enButtonRB1)) {
	//  	//7
	//  	auto bentoInfo = std::make_unique<ItemInfo>();
	//  	bentoInfo->m_type = enItemType_Bento;
	//  
	//  	m_itemInfoList.push_back(std::move(bentoInfo));
	//  }
	//  
	//  if (g_pad[0]->IsTrigger(enButtonRB1)) {
	//  	//8
	//  	auto sushiInfo = std::make_unique<ItemInfo>();
	//  	sushiInfo->m_type = enItemType_Sushi;
	//  
	//  	m_itemInfoList.push_back(std::move(sushiInfo));
	//  
	//  }
	//  
	//  
	//  /**
	//   * TODO;
	//   * おにぎり/弁当/寿司の要素を確認して
	//   * 消したいアイテムを選べられるようにしたい
	//   */
	//  if (g_pad[0]->IsTrigger(enButtonLeft)) {
	//  	auto emptyInfo = std::make_unique<ItemInfo>();
	//  	emptyInfo->m_type = enItemType_Empty;
	//  
	//  	auto it = std::find(m_itemInfoList.begin(), m_itemInfoList.end(), m_itemInfoList.at(m_currentIndex));
	//  	m_itemInfoList.erase(it);
	//  
	//  	//m_itemInfoList.push_back(std::move(emptyInfo));
	//  
	//  }
}