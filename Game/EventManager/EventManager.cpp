#include "stdafx.h"
#include "EventManager.h"

//インスタンスの初期化
EventManager* EventManager::m_eventManagerInstance = nullptr;

void EventManager::NotifyListener(const IEventDataStruct& event)
{
	//比較のためtype_indexに変換
	std::type_index typeIndex(typeid(event));
	//該当するイベントリスナーを探索
	auto findIter = m_eventListnersMap.find(typeIndex);

	if (findIter == m_eventListnersMap.end())
	{
		return;
	}

	//リスナーを順番に起動
	for (auto& ptr : findIter->second)
	{
		//関数のアドレスを取り出す
		auto& condition = ptr.first;
		auto& handler = ptr.second;

		//条件関数がtrueを返した場合、処理関数を起動する
		if (condition())
		{
			handler(event);
		}
	}
}