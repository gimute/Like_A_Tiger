#pragma once

#include "Actor\YakuzaComponents\YakuzaType.h"
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
	void Register(EnemyYakuzaType type, CreateFunc func)
	{
		m_registry[type] = func;
	}
	//ì¬
	std::unique_ptr<IEnemyAi> Create(EnemyYakuzaType type,YakuzaStateMachine* hasStateMachine)
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
	std::unordered_map<EnemyYakuzaType, CreateFunc> m_registry;
};

