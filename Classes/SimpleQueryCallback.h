#ifndef __QUERY_CALLBACK_H__ 
#define __QUERY_CALLBACK_H__

#include "Box2D\Box2D.h"
class SimpleQueryCallback : public b2QueryCallback
{
public:
	b2Vec2 pointToTest;
	b2Fixture *fixtureFound;

	SimpleQueryCallback(const b2Vec2 &point);
	bool ReportFixture(b2Fixture *fixture);
};

#endif