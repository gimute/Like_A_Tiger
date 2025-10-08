#pragma once

#include "EventManager\IEventDataStruct.h"

struct EventTest : public IEventDataStruct
{

	int testValue = 0;

};

struct EventTest2 : public IEventDataStruct
{
	int testValue = 0;
};