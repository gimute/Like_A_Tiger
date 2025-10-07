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
	/// 処理ペア
	/// </summary>
	using EventFuncPair = std::pair<ConditionFunc, HandlerFunc>;
	///シングルトーンパターンを採用するため、コンストラクタをprivateにする
	/// <summary>
	/// インスタンス
	/// </summary>
	static EventManager* m_eventManagerInstance;
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EventManager() = default;
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
		ObjectType* object,
		std::function<bool(ObjectType* object)> condition,
		std::function<void(ObjectType* object, const EventType&)> handler
	)
	{
		//型識別子を取得
		std::type_index typeIndex(typeid(EventType));

		//配列の末尾にイベントリスナーを追加
		m_eventListnersMap[typeIndex].emplace_back(
			[object, condition]()->bool
			{
				//関数が起動可能かどうかを返す
				return condition(object);
			},
			[object, handler](const IEventDataStruct& event)
			{
				//イベントリスナー通知処理
				handler(object, dynamic_cast<const EventType&>(event));
			}
		);
	}
	/// <summary>
	/// 指定されたイベントをリスナーに通知する関数
	/// </summary>
	/// <param name="event"></param>
	void NotifyListener(const IEventDataStruct& event);
private:
	/// <summary>
	/// イベントリスナー登録配列
	/// </summary>
	std::unordered_map <
		//構造体名を型識別子に変換する
		std::type_index, 
		//イベント関数ペアのベクター
		std::vector<EventFuncPair>
	> m_eventListnersMap;
};

