#include"BoundaryCheckSystem.h"
#include"EntityMananger.h"
using namespace ShapeWar ;

BoundaryCheckSystem::BoundaryCheckSystem(int priority)
	:System(priority)
{

}

BoundaryCheckSystem::~BoundaryCheckSystem()
{

}

void BoundaryCheckSystem::enter()
{

}// end for enter

void BoundaryCheckSystem::excute(float dt)
{
	//Get all PositionComponent list
	EntityManager::Component_List* pPos = EntityManager::getEntityManager()->getComponentList(COMPONENT_POSITION);

	//Get all the EntityTypeComponent list
	EntityManager::Component_List* pType = EntityManager::getEntityManager()->getComponentList(COMPONENT_ENTITY_TYPE);
	
	unsigned int size = EntityManager::getEntityManager()->getEntitySize();

	//Find the Player's health Component
	unsigned int player_entity = -1 ;
	for(int i = 0 ; i < size ; i ++)
	{
		if(((EntityTypeComponent*)(*pType)[i])->type == EntityTypeComponent::PLAYER)
		{
			player_entity = i ;
			break ;
		}
	}// end for

	HealthComponent * health = (HealthComponent*)EntityManager::getEntityManager()->getComponent(COMPONENT_HEALTH, player_entity);

	//Check if the entity is out of the screen
	for(unsigned int i = 0 ; i < size ; )
	{
		if(((EntityTypeComponent*)(*pType)[i])->type == EntityTypeComponent::SPHERE_BALL)
		{
			if(((PositionComponent*)(*pPos)[i])->y > 480)
			{
				EntityManager::getEntityManager()->removeEntity(i);
				size -= 1 ;
				continue ;
			}
		}// end if for sphere ball
		else
		{
			if(((PositionComponent*)(*pPos)[i])->y < 0)
			{
				EntityManager::getEntityManager()->removeEntity(i);
				size -= 1 ;
				health->health-- ;
				continue ;
			}
		}

		i ++ ;
	}// end for
}// end for excute

void BoundaryCheckSystem::exit()
{
	
}// end for exit