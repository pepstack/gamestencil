#include"EntityCreator.h"
#include"EntityMananger.h"
#include"GameInfo.h"
using namespace ShapeWar ;

EntityCreator::EntityCreator()
{

}

EntityCreator::~EntityCreator()
{

}

_int64 EntityCreator::createRedCube(float vx, float vy)
{
	EntityManager* manager = EntityManager::getEntityManager();
	_int64 entity = manager->createEntity();

	//Create the entity flag
	_int64 flag  = COMPONENT_NONE ;

	//Add the Render Component
	RenderComponent * pRender = new RenderComponent();
	pRender->sprite = new CCSprite();
	pRender->sprite->initWithFile("RedCube.png");
	manager->addComponent(pRender, COMPONENT_RENDER, entity);
	flag |= COMPONENT_RENDER ;

	//Add the Position Component
	PositionComponent * pPos = new PositionComponent();
	pPos->x = rand() % 320 ;
	pPos->y = 480 ;
	manager->addComponent(pPos, COMPONENT_POSITION, entity);
	flag |= COMPONENT_POSITION ;

	//Add the Velocity Component
	VelocityComponent * pVelocity = new VelocityComponent();
	pVelocity->vx = vx ;
	pVelocity->vy = vy ;
	manager->addComponent(pVelocity, COMPONENT_VELOCITY, entity);
	flag |= COMPONENT_VELOCITY ;

	//Add the Health Component
	HealthComponent * pHealth = new HealthComponent();
	pHealth->health = 1 ;
	manager->addComponent(pHealth, COMPONENT_HEALTH, entity);
	flag |= COMPONENT_HEALTH ;

	//Add the Collid Component
	CollidableComponent* pCollid = new CollidableComponent();
	pCollid->min_x = pPos->x - 16 ;
	pCollid->min_y = pPos->y - 16 ;
	pCollid->max_x = pPos->x + 16 ;
	pCollid->max_y = pPos->y + 16 ;
	manager->addComponent(pCollid, COMPONENT_COLLID, entity);
	flag |= COMPONENT_COLLID ;

	//Add the EntityType Component
	EntityTypeComponent* pEntityType = new EntityTypeComponent();
	pEntityType->type = EntityTypeComponent::RED_CUBE ;
	manager->addComponent(pEntityType, COMPONENT_ENTITY_TYPE, entity);
	flag |= COMPONENT_ENTITY_TYPE ;

	//Add the entity flag
	manager->setEntityFlag(entity, flag);

	return entity ;
}// end for createRedCube

_int64 EntityCreator::createPurpleCube(float vx, float vy)
{
	EntityManager* manager = EntityManager::getEntityManager();
	_int64 entity = manager->createEntity();

	//Create the flag
	_int64 flag = COMPONENT_NONE ;

	//Add the Render Component
	RenderComponent * pRender = new RenderComponent();
	pRender->sprite = new CCSprite();
	pRender->sprite->initWithFile("PurpleCube.png");
	manager->addComponent(pRender, COMPONENT_RENDER, entity);
	flag |= COMPONENT_RENDER ;

	//Add the Position Component
	PositionComponent * pPos = new PositionComponent();
	pPos->x = rand() % 320 ;
	pPos->y = 480 ;
	manager->addComponent(pPos, COMPONENT_POSITION, entity);
	flag |= COMPONENT_POSITION ;

	//Add the Velocity Component
	VelocityComponent * pVelocity = new VelocityComponent();
	pVelocity->vx = vx ;
	pVelocity->vy = vy ;
	manager->addComponent(pVelocity, COMPONENT_VELOCITY, entity);
	flag |= COMPONENT_VELOCITY ;

	//Add the Health Component
	HealthComponent * pHealth = new HealthComponent();
	pHealth->health = 3 ;
	manager->addComponent(pHealth, COMPONENT_HEALTH, entity);
	flag |= COMPONENT_HEALTH ;

	//Add the Collid Component
	CollidableComponent* pCollid = new CollidableComponent();
	pCollid->min_x = pPos->x - 16 ;
	pCollid->min_y = pPos->y - 16 ;
	pCollid->max_x = pPos->x + 16 ;
	pCollid->max_y = pPos->y + 16 ;
	manager->addComponent(pCollid, COMPONENT_COLLID, entity);
	flag |= COMPONENT_COLLID ;

	//Add the EntityType Component
	EntityTypeComponent* pEntityType = new EntityTypeComponent();
	pEntityType->type = EntityTypeComponent::PURPLE_CUBE ;
	manager->addComponent(pEntityType, COMPONENT_ENTITY_TYPE, entity);
	flag |= COMPONENT_ENTITY_TYPE ;

	//Add the entity flag
	manager->setEntityFlag(entity, flag);

	return entity ;
}// end for createPurpleCube

_int64 EntityCreator::createOrangeCube(float vx, float vy)
{
	EntityManager* manager = EntityManager::getEntityManager();
	_int64 entity = manager->createEntity();

	//Create the flag
	_int64 flag = COMPONENT_NONE ;

	//Add the Render Component
	RenderComponent * pRender = new RenderComponent();
	pRender->sprite = new CCSprite();
	pRender->sprite->initWithFile("OrangeCube.png");
	manager->addComponent(pRender, COMPONENT_RENDER, entity);
	flag |= COMPONENT_RENDER ;

	//Add the Position Component
	PositionComponent * pPos = new PositionComponent();
	pPos->x = rand() % 320 ;
	pPos->y = 480 ;
	manager->addComponent(pPos, COMPONENT_POSITION, entity);
	flag |= COMPONENT_POSITION ;

	//Add the Velocity Component
	VelocityComponent * pVelocity = new VelocityComponent();
	pVelocity->vx = vx ;
	pVelocity->vy = vy ;
	manager->addComponent(pVelocity, COMPONENT_VELOCITY, entity);
	flag |= COMPONENT_VELOCITY ;

	//Add the Health Component
	HealthComponent * pHealth = new HealthComponent();
	pHealth->health = 2 ;
	manager->addComponent(pHealth, COMPONENT_HEALTH, entity);
	flag |= COMPONENT_HEALTH ;

	//Add the Collid Component
	CollidableComponent* pCollid = new CollidableComponent();
	pCollid->min_x = pPos->x - 16 ;
	pCollid->min_y = pPos->y - 16 ;
	pCollid->max_x = pPos->x + 16 ;
	pCollid->max_y = pPos->y + 16 ;
	manager->addComponent(pCollid, COMPONENT_COLLID, entity);
	flag |= COMPONENT_COLLID ;

	//Add the EntityType Component
	EntityTypeComponent* pEntityType = new EntityTypeComponent();
	pEntityType->type = EntityTypeComponent::ORANGE_CUBE ;
	manager->addComponent(pEntityType, COMPONENT_ENTITY_TYPE, entity);
	flag |= COMPONENT_ENTITY_TYPE ;

	//Add the entity flag
	manager->setEntityFlag(entity, flag);

	return entity ;
}// end for createOrangeCube

_int64 EntityCreator::createGreenCube(float vx, float vy)
{
	EntityManager* manager = EntityManager::getEntityManager();
	_int64 entity = manager->createEntity();

	//Create flag
	_int64 flag = COMPONENT_NONE ;

	//Add the Render Component
	RenderComponent * pRender = new RenderComponent();
	pRender->sprite = new CCSprite();
	pRender->sprite->initWithFile("GreenCube.png");
	manager->addComponent(pRender, COMPONENT_RENDER, entity);
	flag |= COMPONENT_RENDER ;

	//Add the Position Component
	PositionComponent * pPos = new PositionComponent();
	pPos->x = rand() % 320 ;
	pPos->y = 480 ;
	manager->addComponent(pPos, COMPONENT_POSITION, entity);
	flag |= COMPONENT_POSITION ;

	//Add the Velocity Component
	VelocityComponent * pVelocity = new VelocityComponent();
	pVelocity->vx = vx ;
	pVelocity->vy = vy ;
	manager->addComponent(pVelocity, COMPONENT_VELOCITY, entity);
	flag |= COMPONENT_VELOCITY ;

	//Add the Health Component
	HealthComponent* pHealth = new HealthComponent();
	pHealth->health = 1 ;
	manager->addComponent(pHealth, COMPONENT_HEALTH, entity);
	flag |= COMPONENT_HEALTH ;

	//Add the Collid Component
	CollidableComponent* pCollid = new CollidableComponent();
	pCollid->min_x = pPos->x - 16 ;
	pCollid->min_y = pPos->y - 16 ;
	pCollid->max_x = pPos->x + 16 ;
	pCollid->max_y = pPos->y + 16 ;
	manager->addComponent(pCollid, COMPONENT_COLLID, entity);
	flag |= COMPONENT_COLLID ;

	//Add the EntityType Component
	EntityTypeComponent* pEntityType = new EntityTypeComponent();
	pEntityType->type = EntityTypeComponent::GREEN_CUBE ;
	manager->addComponent(pEntityType, COMPONENT_ENTITY_TYPE, entity);
	flag |= COMPONENT_ENTITY_TYPE ;

	//Add the entity flag
	manager->setEntityFlag(entity, flag);

	return entity ;
}// end for createGreenCube

_int64 EntityCreator::createSphereBall(float x, float y)
{
	EntityManager* manager = EntityManager::getEntityManager();
	_int64 entity = manager->createEntity();

	//Create the flag
	_int64 flag = COMPONENT_NONE ;

	//Add the Render Component
	RenderComponent * pRender = new RenderComponent();
	pRender->sprite = new CCSprite();
	pRender->sprite->initWithFile("SphereBall.png");
	pRender->sprite->setPosition(ccp(x,y));
	manager->addComponent(pRender, COMPONENT_RENDER, entity);
	flag |= COMPONENT_RENDER ;

	//Add the Health Component
	HealthComponent* pHealth = new HealthComponent();
	pHealth->health = 1 ;
	manager->addComponent(pHealth, COMPONENT_HEALTH, entity);
	flag |= COMPONENT_HEALTH ;

	//Add the Position Component
	PositionComponent* pPos = new PositionComponent();
	pPos->x = x ;
	pPos->y = y ;
	manager->addComponent(pPos, COMPONENT_POSITION, entity);
	flag |= COMPONENT_POSITION ;

	//Add the Collid Component
	CollidableComponent* pCollid = new CollidableComponent();
	pCollid->min_x = pPos->x - 16 ;
	pCollid->min_y = pPos->y - 16 ;
	pCollid->max_x = pPos->x + 16 ;
	pCollid->max_y = pPos->y + 16 ;
	manager->addComponent(pCollid, COMPONENT_COLLID, entity);
	flag |= COMPONENT_COLLID ;

	//Add the EntityType Component
	EntityTypeComponent* pEntityType = new EntityTypeComponent();
	pEntityType->type = EntityTypeComponent::SPHERE_BALL ;
	manager->addComponent(pEntityType, COMPONENT_ENTITY_TYPE, entity);
	flag |= COMPONENT_ENTITY_TYPE ;

	//Add the entity flag
	manager->setEntityFlag(entity, flag);

	return entity ;
}// end for createSphereBall

_int64 EntityCreator::createPlayer()
{
	//Create a sphere ball with it
	createSphereBall(160,20);

	EntityManager* manager = EntityManager::getEntityManager();
	_int64 entity = manager->createEntity();

	//Create the flag
	_int64 flag = COMPONENT_NONE ;

	//Add the Render Component
	RenderComponent * pRender = new RenderComponent();
	pRender->sprite = new CCSprite();
	pRender->sprite->initWithFile("Shooter.png");
	manager->addComponent(pRender, COMPONENT_RENDER, entity);
	flag |= COMPONENT_RENDER ;

	//Add the Health Component
	HealthComponent* pHealth = new HealthComponent();
	pHealth->health = GameInfo::getGameInfo()->MAX_HEALTH_PLAYER ;
	manager->addComponent(pHealth, COMPONENT_HEALTH, entity);
	flag |= COMPONENT_HEALTH ;

	//Add the Position Component
	PositionComponent* pPos = new PositionComponent();
	pPos->x = 160 ;
	pPos->y = 10 ;
	manager->addComponent(pPos, COMPONENT_POSITION, entity);
	flag |= COMPONENT_POSITION ;

	//Add the EntityType Component
	EntityTypeComponent* pEntityType = new EntityTypeComponent();
	pEntityType->type = EntityTypeComponent::PLAYER ;
	manager->addComponent(pEntityType, COMPONENT_ENTITY_TYPE, entity);
	flag |= COMPONENT_ENTITY_TYPE ;

	//Add the entity flag
	manager->setEntityFlag(entity, flag);

	return entity ;
}// end for createPlayer