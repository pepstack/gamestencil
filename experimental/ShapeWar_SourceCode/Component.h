//------------------------------------------------------------------
// declaration	: Copyright (c), by XJ , 2014 . All right reserved .
// brief		: This file will define the Component base class of the 
//                Entity-Component-System.
// author		: XJ
// date			: 2014 / 6 / 8
// version		: 1.0
//-------------------------------------------------------------------
#pragma once
#include<cocos2d.h>
using namespace cocos2d ;

namespace ShapeWar
{
#define COMPONENT_NONE 0x0

class Component
{
public:
	Component(){}
	virtual ~Component(){}
};

/**
* Define the Render Component
*/
#define COMPONENT_RENDER (1 << 1)
class RenderComponent: public Component
{
public:
	RenderComponent(){}
	~RenderComponent()
	{
		sprite->removeFromParentAndCleanup(true);
		delete sprite ;
	}

public:
	CCSprite* sprite ;
};

/**
* Define the Position Component
*/
#define COMPONENT_POSITION (1 << 2 )
class PositionComponent: public Component
{
public:
	PositionComponent(){}
	~PositionComponent(){}

public:
	float x ;
	float y ;
};

/**
* Define the Velocity Component
*/
#define COMPONENT_VELOCITY (1 << 3)
class VelocityComponent: public Component
{
public:
	VelocityComponent(){}
	~VelocityComponent(){}

public:
	float vx ;
	float vy ;
};

/**
* Define the Health Component
*/
#define COMPONENT_HEALTH (1 << 4)
class HealthComponent: public Component
{
public:
	HealthComponent(){}
	~HealthComponent(){}

public:
	unsigned int health ;
};

/**
* Define the Collidable Component
* brief	: Use the AABB's Min-Max representation
*/
#define COMPONENT_COLLID (1 << 5)
class CollidableComponent:public Component
{
public:
	CollidableComponent(){}
	~CollidableComponent(){}

public:
	float min_x ;
	float min_y ;
	float max_x ;
	float max_y ;
};

/**
* Define the EntityType component
* brief	: This component will indicate which type the entity is.
*/
#define COMPONENT_ENTITY_TYPE (1 << 6)
class EntityTypeComponent: public Component
{
public:
	EntityTypeComponent(){}
	~EntityTypeComponent(){}

public:
	static const unsigned int RED_CUBE = (1 << 1) ;
	static const unsigned int PURPLE_CUBE = (1 << 2) ;
	static const unsigned int ORANGE_CUBE = (1 << 3) ;
	static const unsigned int GREEN_CUBE = (1 << 4) ;
	static const unsigned int SPHERE_BALL = (1 << 5) ;
	static const unsigned int PLAYER = (1 << 6) ;

public:
	unsigned int type ;
};

/**
* Define the AnimateComponent
*/
#define COMPONENT_ANIMATE (1 << 7)
class AnimateComponent:public Component
{
public:
	AnimateComponent(){}
	~AnimateComponent(){}

public:
	cocos2d::CCAnimate* animate ;
	unsigned frames ;
};

};