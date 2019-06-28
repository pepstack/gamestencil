#include"AnimateSystem.h"
#include"EntityMananger.h"
using namespace ShapeWar ;

AnimateSystem::AnimateSystem(int priority)
	:System(priority)
{

}

AnimateSystem::~AnimateSystem()
{

}

void AnimateSystem::enter()
{

}// end for enter

void AnimateSystem::excute(float  dt)
{
	//Get all the Animate Component List
	EntityManager::Component_List* pAnimate = EntityManager::getEntityManager()->getComponentList(COMPONENT_ANIMATE);

	//Iterate all the Animate
	EntityManager::Component_List::iterator it = pAnimate->begin();
	unsigned int entity = 0 ;
	for(; it != pAnimate->end();)
	{
		AnimateComponent* animate = (AnimateComponent*)*it ;
		it ++ ;

		if(animate != NULL)
		{
			if(animate->animate->getTarget() == NULL)
			{
				RenderComponent* render = (RenderComponent*)EntityManager::getEntityManager()->getComponent(COMPONENT_RENDER, entity);
				render->sprite->runAction(animate->animate);
			}//end if

			animate->frames -- ;
			if(animate->frames == 0)
			{
				RenderComponent* render = (RenderComponent*)EntityManager::getEntityManager()->getComponent(COMPONENT_RENDER, entity);
				render->sprite->stopAction(animate->animate);
				animate->animate->release();
				EntityManager::getEntityManager()->removeComponent(COMPONENT_ANIMATE, entity);
			}// end if
		}// end if

		entity ++ ;
	}// end for
}// end for excute

void AnimateSystem::exit()
{

}// end for exit

