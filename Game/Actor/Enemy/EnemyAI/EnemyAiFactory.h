#pragma once

#include "Actor\Enemy\EnemyType.h"
#include "Actor\Enemy\EnemyAI\IEnemyAi.h"

class IEnemyAi;

class EnemyAiFactory
{
public: 
	//ŠÖ”–½–¼
	using CreateFunc = std::function<std::unique_ptr<IEnemyAi>(YakuzaStateMachine*)>;
	//ƒCƒ“ƒXƒ^ƒ“ƒXæ“¾
	static EnemyAiFactory& GetInstance()
	{
		static EnemyAiFactory inst;
		return inst;
	}
	//“o˜^
	void Register(EnemyType type, CreateFunc func)
	{
		m_registry[type] = func;
	}
	//ì¬
	std::unique_ptr<IEnemyAi> Create(EnemyType type,YakuzaStateMachine* hasStateMachine)
	{
		auto it = m_registry.find(type);
		if (it != m_registry.end())
		{
			return it->second(hasStateMachine);
		}
		return nullptr;
	}
private:
	//“o˜^‚³‚ê‚½Ai‚ÌƒŠƒXƒg
	std::unordered_map<EnemyType, CreateFunc> m_registry;
};

