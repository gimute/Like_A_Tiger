#pragma once
#include <vector>
#include <fstream>


//TODO; 一つ一つ処理を見てみよう
// 遠回りだと思うけどそれが近道

//問題点: save/loadをいつ呼ぶか　
// アイテム追加されるた度にセーブして
// 正味、メニュー画面を開けた時にロード呼ぶのがいいのかなと

/* セーブデータを管理するクラス */
class SaveManager
{
private:
	SaveManager() {}
	~SaveManager() {}

	//保存するファイル名
	const std::string SAVE_FILE_NAME = "Assets/save/saveData.json";

public:
	// コピー禁止
	SaveManager(const SaveManager&) = delete;
	SaveManager& operator = (const SaveManager&) = delete;

	// インスタンス取得
	static SaveManager& GetInstance() {
		static SaveManager instance;
		return instance;
	}

	// セーブ実行
	void Save();

	// ロード実行
	void Load();

//private:
//	static SaveManager* m_instance;

	//  // セーブ実行
	//  void Save() {
	//  	// インベントリから現在のアイテムリストを取得(Inventoryクラス側に取得関数が必要)
	//  	std::vector<EnItemType> itemsToSave = Inventory::Create()->GetItemInfoList();
	// 
	//  	//output file stream: ファイルへの出力
	//  	std::ofstream file(SAVE_FILE_NAME, std::ios::binary);
	//  	if (!file) return;
	//  
	//  	//アイテムの個数を書き込み
	//  	size_t size = itemsToSave.size();
	// 
	//  	//korenani
	//  	// reinterpret_cast ビット表現せずに型だけ変更 安全性最低やん
	//  	// なぜこれを使ったの？
	//  	// →バイナリ書き込みのルールだから
	//  	file.write(reinterpret_cast<const char*>(&size), sizeof(size));
	//  
	//  	//アイテムの中身を書き込み
	//  	file.write(reinterpret_cast<const char*>(itemsToSave.data()), sizeof(EnItemType) * size);
	//  }
	//  
	//  // ロード実行
	//  void Load() {
	//  	//input file stream: ファイルからの入力
	//  	std::ifstream file(SAVE_FILE_NAME, std::ios::binary);
	//  	if (!file) return;
	//  
	//  	size_t size = 0;
	//  	file.read(reinterpret_cast<char*>(&size), sizeof(size));
	//  
	//  	std::vector<EnItemType> loadedItems(size);
	//  	file.read(reinterpret_cast<char*>(loadedItems.data()), sizeof(EnItemType) * size);
	//  
	//  	//TODO: ステートマシン内で情報を取得したいがためにその情報のクラスを生成するのはいかがなものか
	//  	//インベントリにデータを反映
	//  	Inventory::Create()->LoadItemList(loadedItems);
	//  }
};

