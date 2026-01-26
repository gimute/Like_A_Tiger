#include "stdafx.h"
#include "SaveManager.h"
#include "Inventory//Inventory.h"
#include <fstream>
#include "Json/json.hpp"

using json = nlohmann::json;

namespace {
	// 保存するファイル名
	const std::string SAVE_FILE_NAME = "Assets/save/saveData.bin";
}
void SaveManager::Save()
{
	// インベントリから現在のアイテムを取得
	Inventory* inventory = Inventory::GetInstance();
	
	// インベントリが存在しない場合は何もしない
	if (!inventory) return;

	// ロードで使う、ロッカーの配置情報を取得
	std::vector<int> items = inventory->GetItemInfoList();
	
	// アイテムごとの個数を集計
	int countOnigiri = inventory->GetItemCount(enItemType_Onigiri);
	int countBento = inventory->GetItemCount(enItemType_Bento);
	int countSushi = inventory->GetItemCount(enItemType_Sushi);

	// JSONオブジェクトを作成
	json j;

	// 配置データ（復元用）
	j["inventoy"] = items;

	// 個数データ（確認用 Summary）
	j["summary"]["Onigiri"] = countOnigiri;
	j["summary"]["Bento"] = countBento;
	j["summary"]["Sushi"] = countSushi;

	// ファイルに書き込む
	std::ofstream file(SAVE_FILE_NAME);
	if (file) {
		// dump(4) でインデント（字下げ）を付けて見やすく保存
		file << j.dump(4);
	}


	//  std::vector<EnItemType> items;
	//  items.reserve(itemInts.size());
	//  for (int i : itemInts)
	//  {
	//  	items.push_back(static_cast<EnItemType>(i));
	//  }
	//  
	//  // JSONオブジェクトを作る
	//  json j;
	//  
	//  // 配列データをJSON形式に変換
	//  j["inventory"] = items;
	//  
	//  // ファイルに書き込む
	//  std::ofstream file("Assets/save/saveData.json");
	//  
	//  //インデントを4スペースにして見やすくした
	//  file << j.dump(4);

	//  // インベントリから現在のアイテムを取得
	//  std::vector<EnItemType> itemsToSave = Inventory::Create()->GetItemInfoList();
	//  // ファイルへの出力
	//  std::ofstream file(SAVE_FILE_NAME, std::ios::binary);
	//  if (!file) { return; }
	//  
	//  // 現在の所持アイテム数を調べる
	//  size_t size = itemsToSave.size();
	//  // reinterpret_castは変数size のメモリ領域を生のバイト列としてファイルに書き込むためのキャスト
	//  file.write(reinterpret_cast<const char*>(&size), sizeof(size));
	//  
	//  // アイテムの中身を一気に書き込み
	//  file.write(reinterpret_cast<const char*>(itemsToSave.data()), sizeof(EnItemType) * size);
}

void SaveManager::Load()
{
	// ファイルを開く
	std::ifstream file(SAVE_FILE_NAME);
	if (!file) { return; }
	if (!file.is_open())
	{
		return;
	}
	/**
	* TODO
	* 一度コード見て自分の言葉にかみ砕いてみる
	* ⇒そして、AIに投げてあっっているか確認
	* 
	 * なんぞやワード
	 * try-catch
	 * 
	 */

	// 反映先がないなら終了
	if (!Inventory::GetInstance()) return;

	try {
		json j;
		file >> j;



		// JSONデータが配列（"inventory"）を持っているか確認
		if (j.contains("inventory") && j["inventory"].is_array()) {
			// int型の配列として取り出す
			std::vector<int> items = j["inventory"].get<std::vector<int>>();

			// Inventoryに反映（ロッカーの中身が元通り）
			Inventory::GetInstance()->LoadItemList(items);
		}

		// "summary"　はロード時には使いません
		//　ろっかーの状態さえ復元できれば、個数は自動的に合うから
	}
	catch (json::exception& e) {

	}

	//   // ファイルからJSONデータを読み込む
	//   json j;
	//   file >> j;
	//   
	//   // データを配列として取り出す
	//   std::vector<int> itemInts = j["inventory"].get<std::vector<int>>();
	//   std::vector<int> items;
	//   items.reserve(itemInts.size());
	//   for (int i : itemInts)
	//   {
	//   	items.push_back(static_cast<int>(i));
	//   }
	//   
	//   // Inventoryに反映
	//   Inventory::GetInstance()->LoadItemList(items);
}
