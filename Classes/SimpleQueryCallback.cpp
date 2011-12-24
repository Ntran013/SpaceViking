#include "SimpleQueryCallback.h"


SimpleQueryCallback::SimpleQueryCallback(const b2Vec2 &point)
{
	pointToTest = point;
	fixtureFound = NULL;
}

bool SimpleQueryCallback::ReportFixture(b2Fixture *fixture)
{
	b2Body* body = fixture->GetBody();
	if (body->GetType() == b2_dynamicBody) 
	{
		if (fixture->TestPoint(pointToTest)) 
		{
			fixtureFound = fixture;
			return false;
		}
	}
	return true;
}
