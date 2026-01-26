#pragma once
#include "Inventory/Types.h"

namespace {
	/** インベントリの最大スロット数（ロッカーの数） */
	const int MAX_INVENTORY_SLOTS = 20;
}

/**
 * たぶん1個
 * 外から作られないようにと１個だけにする処理だけして、取得はさせないようにしようかな
 */
class Inventory
{
private:
	//アイテムデータの実体
	std::vector<std::unique_ptr<ItemInfo>> m_itemInfoList;

private:
	Inventory();
	~Inventory();

public:
	// シングルトン
	static Inventory* Create();
	static void Delete();
	static Inventory* GetInstance() { return m_instance; }

	/** デバック操作用 */
	void Update();

public:
	/** 全スロット走査用 */
	void ForEach(const std::function<void(int index,ItemInfo*)>& func)
	{
		for (int i = 0; i < (int)m_itemInfoList.size(); ++i) {
			func(i, m_itemInfoList[i].get());
		}
	}


public:
	/** アイテムを追加 */
	int AddItem(EnItemType type);

	/** 指定した場所のアイテムを削除 */
	void RemoveItem(int index);

	/** 指定した場所のアイテム情報を取得 */
	ItemInfo* GetItemInfo(int index);

	// 現在のインベントリ内のアイテムリストを取得
	std::vector<int> GetItemInfoList() const;

	// JSON保存用：ロード時にアイテムリストを一括設定
	void LoadItemList(const std::vector<int>& types);

	//指定した種類のアイテムを数える
	int GetItemCount(EnItemType type) const;

	//  static Inventory* Create()
	//  {
	//  	if (m_instance == nullptr)
	//  	{
	//  		m_instance = new Inventory();
	//  	}
	//  	return m_instance;
	//  }

	//  static void Delete()
	//  {
	//  	if (m_instance) {
	//  		delete m_instance;
	//  		m_instance = nullptr;
	//  	


private:
	static Inventory* m_instance;

};


// いつもはcppでするけど、ここで書いてみたｗ
//Inventory* Inventory::m_instance = nullptr;