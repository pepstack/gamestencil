#include"RenderSystem.h"
#include"EntityMananger.h"
using namespace ShapeWar ;

RenderSystem::RenderSystem(int _priority, CCNode* _scene)
	:System(_priority),
	scene(_scene)
{

}

RenderSystem::~RenderSystem()
{

}

void RenderSystem::enter()
{

}// ed for enter

void RenderSystem::excute(float dt)
{
	unsigned int size = EntityManager::getEntityManager()->getEntitySize();
	for(unsigned int i = 0 ; i < size ; i ++)
	{
		_int64 flag = EntityManager::getEntityManager()->getEntityFlag(i);
		if((flag & (COMPONENT_RENDER | COMPONENT_POSITION)) == (COMPONENT_RENDER | COMPONENT_POSITION))
		{
			RenderComponent* pRender = (RenderComponent*)EntityManager::getEntityManager()->getComponent(COMPONENT_RENDER,i);
			PositionComponent* pPos =  (PositionComponent*)EntityManager::getEntityManager()->getComponent(COMPONENT_POSITION,i);

			if(pRender->sprite->getParent() == NULL)
			{
				EntityTypeComponent* pType = (EntityTypeComponent*)EntityManager::getEntityManager()->getComponent(COMPONENT_ENTITY_TYPE,i);
				if(pType->type != EntityTypeComponent::PLAYER)
				{
					pRender->sprite->runAction(CCRepeatForever::create(CCRotateBy::create(1.0/60, 5)));
					scene->addChild(pRender->sprite);
				}// end for PLAYER
				else
					scene->addChild(pRender->sprite, 10);
			}

			pRender->sprite->setPosition(ccp(pPos->x, pPos->y));
		}
	}// end for sprite

}// end for excute

void RenderSystem::exit()
{
	unsigned int size = EntityManager::getEntityManager()->getEntitySize();
	for(unsigned int i = 0 ; i < size ; i ++)
	{
		RenderComponent* pRender = (RenderComponent*)EntityManager::getEntityManager()->getComponent(COMPONENT_RENDER,i);
		pRender->sprite->stopAllActions();
		pRender->sprite->removeFromParentAndCleanup(true);
	}// end for
}// end for exit