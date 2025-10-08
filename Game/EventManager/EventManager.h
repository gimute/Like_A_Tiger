#pragma once
//型識別の為にtypeindexをインクルード
#include <typeindex>

#include "EventManager\IEventDataStruct.h"

class EventManager
{
private:
	///命名定義
	/// <summary>
	/// 状態確認関数
	/// </summary>
	using ConditionFunc = std::function<bool()>;
	/// <summary>
	/// 処理関数
	/// </summary>
	using HandlerFunc = std::function<void(const IEventDataStruct&)>;
	/// <summary>
	/// リスナーの関数、オブジェクトを格納する構造体
	/// </summary>
	struct ListenerEntry
	{
		/// <summary>
		/// リスナーオブジェクトの弱参照ポインタ
		/// </summary>
		std::weak_ptr<void> m_objecyPtr;
		/// <summary>
		/// 起動判定関数
		/// </summary>
		ConditionFunc m_conditionFunc;
		/// <summary>
		/// 通知処理関数
		/// </summary>
		HandlerFunc m_handlerFunc;
		/// <summary>
		/// イベントの通番
		/// </summary>
		int m_eventNo = 0;
	};
	/// <summary>
	/// リスナーの配列
	/// </summary>
	using ListenerList = std::vector<ListenerEntry>;
	///シングルトーンパターンを採用するため、コンストラクタをprivateにする
	/// <summary>
	/// インスタンス
	/// </summary>
	static EventManager* m_eventManagerInstance;
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EventManager()
	{
		m_eventListnersMap.clear();
	}
	/// <summary>
	/// コピー禁止にする
	/// </summary>
	/// <param name=""></param>
	EventManager(const EventManager&) = delete;
	/// <summary>
	/// 代入禁止にする
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	EventManager& operator=(const EventManager&) = delete;
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~EventManager() = default;
	/// <summary>
	/// イベントマネージャーインスタンス取得関数
	/// </summary>
	/// <returns></returns>
	static EventManager* GetEventManagerInstance()
	{
		if (m_eventManagerInstance == nullptr)
		{
			m_eventManagerInstance = new EventManager();
		}
		return m_eventManagerInstance;
	}
	/// <summary>
	/// イベントマネージャーにリスナーを登録する関数
	/// </summary>
	/// <typeparam name="EventType"></typeparam>
	/// <typeparam name="ObjectType"></typeparam>
	/// <param name="object"></param>
	/// <param name="condition"></param>
	/// <param name="handler"></param>
	template<typename EventType,typename ObjectType>
	inline void RegisterListener(
		std::shared_ptr<ObjectType> object,
		std::function<bool(ObjectType*)> condition,
		std::function<void(ObjectType*, const EventType&)> handler,
		int eventNo = 0
	)
	{
		//型識別子を取得
		std::type_index typeIndex(typeid(EventType));

		ListenerEntry entry;
		entry.m_objecyPtr = object;

		//条件関数
		entry.m_conditionFunc = 
			[weakObj = std::weak_ptr<ObjectType>(object),condition]()
			{
				if (auto obj = weakObj.lock())
				{
					return condition(obj.get());
				}
				return false;
			};
		//処理関数
		entry.m_handlerFunc =
			[weakObj = std::weak_ptr<ObjectType>(object),
			handler](const IEventDataStruct& event)
			{
				if (auto obj = weakObj.lock())
				{
					handler(obj.get(), dynamic_cast<const EventType&>(event));
				}
			};

		//通番を代入
		entry.m_eventNo = eventNo;

		//実行順を決めるため、通番順に挿入する
		auto vecPos = std::lower_bound(
			m_eventListnersMap[typeIndex].begin(),
			m_eventListnersMap[typeIndex].end(),
			eventNo,
			[](const ListenerEntry& event, size_t id)
			{
				return event.m_eventNo < id;
			});

		//配列に追加
		m_eventListnersMap[typeIndex].insert(vecPos,std::move(entry));
	}
	/// <summary>
	/// 指定されたイベントをリスナーに通知する関数
	/// </summary>
	/// <param name="event"></param>
	bool NotifyListener(const IEventDataStruct& event);
	/// <summary>
	/// 指定されたオブジェクトのリスナー登録を解除する関数
	/// </summary>
	/// <typeparam name="EventType"></typeparam>
	/// <typeparam name="ObjectType"></typeparam>
	/// <param name="object"></param>
	template<typename EventType,typename ObjectType>
	inline bool UnRegisterListener(
		std::shared_ptr<ObjectType> object,
		uint64_t eventNo = 0
	)
	{
		std::type_index typeIndex(typeid(EventType));

		auto findIter = m_eventListnersMap.find(typeIndex);

		if (findIter == m_eventListnersMap.end())
		{
			return false;
		}

		auto& listners = findIter->second;

		//削除前のサイズを記録
		size_t beforeSize = listners.size();

		//リスナー配列から、指定されたオブジェクトとイベント通番が一致するものを削除する
		listners.erase(
			std::remove_if(listners.begin(),listners.end(),
				[object,eventNo](const ListenerEntry& entry) {

					auto lockedVoid = entry.m_objecyPtr.lock();
					if (!lockedVoid)
					{
						return true;
					}

					//キャストしてポイント比較
					auto locked = std::static_pointer_cast<ObjectType>(lockedVoid);
					return (locked.get() == object.get() && entry.m_eventNo == eventNo);
					/*return !entry.m_objecyPtr.owner_before(object) &&
						!object.owner_before(entry.m_objecyPtr) */ 
				}),
			listners.end()
		);

		//リスナーが空になったら、マップからも削除する
		if (listners.empty())
		{
			m_eventListnersMap.erase(findIter);
		}

		//削除されたかどうかを返す
		return listners.size() != beforeSize;
	}
private:
	/// <summary>
	/// イベントリスナー登録配列
	/// </summary>
	std::unordered_map <
		//構造体名を型識別子に変換する
		std::type_index,
		//イベントリスナーの配列
		ListenerList
	> m_eventListnersMap;
};

