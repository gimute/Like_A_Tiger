#include "stdafx.h"
#include "EventManager.h"

//インスタンスの初期化
EventManager* EventManager::m_eventManagerInstance = nullptr;

/*
	★武田式イベントマネージャーの使い方★

	1.通知時に渡したい変数をメンバに持つ構造体をIEventDataStructから派生させて作成する。
	2.イベントを受け取りたいクラスで、shared_from_this()を使ってshared_ptrを取得できるようにする。
	3.イベントを受け取りたいクラスで、EventManager::GetEventManagerInstance()->RegisterListener<EventType, ObjectType>()
		を呼び出して、イベントリスナーを登録する。
		第一テンプレート引数に1で作成した構造体の型、第二テンプレート引数にイベントを受け取りたいクラスの型を指定する。
		第一引数にshared_from_this()で取得したshared_ptr、第二引数にイベントを受け取る条件を返す関数、
		第三引数にイベントを受け取ったときに実行する関数、第四引数にイベントの通番(実行順)を指定する。
		以下作成例
		EventManager::GetEventManagerInstance()->RegisterListener<EventTest, TestPlayer>(
			self,　shared_from_this()で取得したshared_ptr
			[](TestPlayer* p)
			{
				条件部分
				return true;
			},
			[](TestPlayer* p,const EventTest& e)
			{
				実行部分
				p->testValue = e.testValue;
			},
			0	通番(実行順
		);
	4.イベントを発行したい場所で、1で作成した構造体をメンバ変数、ローカル変数どちらでも良いので作成し、
		必要な変数をセットして、EventManager::GetEventManagerInstance()->NotifyListener()に引数として渡す。
		これで発火したイベントが、登録されているリスナーに通知される。
	5.イベントリスナーの登録を解除したい場合は、EventManager::GetEventManagerInstance()->UnRegisterListener<EventType, ObjectType>()
		を呼び出して、イベントリスナーの登録を解除する。
		第一テンプレート引数に1で作成した構造体の型、第二テンプレート引数にイベントを受け取りたいクラスの型を指定する。
		第一引数にshared_from_this()で取得したshared_ptr、第二引数にイベントの通番(実行順)を指定する。
		以下作成例
		EventManager::GetEventManagerInstance()->UnRegisterListener<EventTest, TestPlayer>(
			self,　shared_from_this()で取得したshared_ptr
			0	通番(実行順、指定した箇所と同じ通番のイベントが登録解除される
		);
*/

//イベントをリスナーに通知する関数
bool EventManager::NotifyListener(const IEventDataStruct& event)
{
	//比較のためtype_indexに変換
	std::type_index typeIndex(typeid(event));
	//該当するイベントリスナーを探索
	auto findIter = m_eventListnersMap.find(typeIndex);

	if (findIter == m_eventListnersMap.end())
	{
		return false;
	}

	auto& listeners = findIter->second;

	//リスナーを順番に起動
	for (auto& listener : listeners)
	{
		if (listener.m_objecyPtr.expired())
		{
			//オブジェクトが解放されていた場合、スキップ
			continue;
		}

		//条件関数がtrueを返した場合、処理関数を起動する
		if (listener.m_conditionFunc())
		{
			listener.m_handlerFunc(event);
		}
	}

	//削除前のサイズを記録
	size_t beforeSize = listeners.size();

	//無効になったリスナーは削除する
	listeners.erase(
		std::remove_if(listeners.begin(),listeners.end(),
		[](const ListenerEntry& entry)
			{
				return entry.m_objecyPtr.expired();
			}),
		listeners.end()
	);

	//リスナーが空になったら、マップからも削除する
	if (listeners.empty())
	{
		m_eventListnersMap.erase(findIter);
	}

	//削除されたかどうかを返す
	return listeners.size() != beforeSize;
}

