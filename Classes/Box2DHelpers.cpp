#include "Box2DHelpers.h"

bool isBodyCollidingWithObjectType( b2Body *body, GameObjectType objectType )
{
	b2ContactEdge *edge = body->GetContactList();
	while (edge)
	{
		b2Contact *contact = edge->contact;
		if (contact->IsTouching())
		{
			b2Fixture *fixtureA = contact->GetFixtureA();
			b2Fixture *fixtureB = contact->GetFixtureB();
			b2Body *bodyA = fixtureA->GetBody();
			b2Body *bodyB = fixtureB->GetBody();
			Box2DSprite *spriteA = (Box2DSprite *) bodyA->GetUserData();
			Box2DSprite *spriteB = (Box2DSprite *) bodyB->GetUserData();
			if ((spriteA != NULL && spriteA->getGameObjectType() == objectType) || (spriteB != NULL && spriteB->getGameObjectType() == objectType))
				return true;
		}

		edge = edge->next;
	}

	return false;
}
