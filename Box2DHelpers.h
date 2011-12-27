#ifndef __BOX2D_HELPERS_H__
#define __BOX2D_HELPERS_H__

#include "Box2D/Box2D.h"
#include "Constants/CommonProtocols.h"
#include "GameObjects\Box2DObjects\Box2DSprite.h"

bool isBodyCollidingWithObjectType(b2Body *body, GameObjectType objectType);

#endif