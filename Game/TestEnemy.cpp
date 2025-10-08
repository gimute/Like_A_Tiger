#include "stdafx.h"
#include "TestEnemy.h"

#include "EventManager\EventManager.h"

#include "EventManager\EventTest.h"

TestEnemy::TestEnemy()
{

}

TestEnemy::~TestEnemy()
{
}

bool TestEnemy::Start()
{
	self = std::make_shared<TestEnemy>();

	EventManager::GetEventManagerInstance()->RegisterListener<EventTest2, TestEnemy>(
		self,
		[](TestEnemy* p)
		{
			return p->testFlag;
		},
		[](TestEnemy* p, const EventTest2& e)
		{
			p->testValue *= e.testValue;
		},
		0
	);


	return true;
}

void TestEnemy::Update()
{
	EventTest e;

	e.testValue = 200;

	EventManager::GetEventManagerInstance()->NotifyListener(e);

	EventTest2 e2;

	e2.testValue = 3;

	EventManager::GetEventManagerInstance()->NotifyListener(e2);
}

void TestEnemy::Render(RenderContext& renderContext)
{
	
}