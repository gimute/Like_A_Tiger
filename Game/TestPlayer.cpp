#include "stdafx.h"
#include "TestPlayer.h"

#include "EventManager\EventTest.h"

#include "EventManager\EventManager.h"

TestPlayer::TestPlayer()
{

}

TestPlayer::~TestPlayer()
{

}

bool TestPlayer::Start()
{

	self = std::make_shared<TestPlayer>();

	EventManager::GetEventManagerInstance()->RegisterListener<EventTest, TestPlayer>(
		self,
		[](TestPlayer* p)
		{
			return false;
		},
		[](TestPlayer* p,const EventTest& e)
		{
			p->testValue = e.testValue;
		},
		1
	);

	EventManager::GetEventManagerInstance()->RegisterListener<EventTest, TestPlayer>(
		self,
		[](TestPlayer* p)
		{
			return p->testFlag;
		},
		[](TestPlayer* p, const EventTest& e)
		{
			p->testValue *= e.testValue;
		},
		0
	);

	EventManager::GetEventManagerInstance()->RegisterListener<EventTest2, TestPlayer>(
		self,
		[](TestPlayer* p)
		{
			return p->testFlag;
		},
		[](TestPlayer* p, const EventTest2& e)
		{
			p->testValue *= e.testValue;
		},
		0
	);

	return true;
}

void TestPlayer::Update()
{
	EventManager::GetEventManagerInstance()->UnRegisterListener<EventTest, TestPlayer>(self,1);

	DeleteGO(this);
}

void TestPlayer::Render(RenderContext& renderContext)
{
	
}