#include"HealthSystem.h"
#include"EntityMananger.h"
#include"GameInfo.h"
using namespace ShapeWar ;

HealthSystem::HealthSystem(int priority)
	:System(priority)
{

}

HealthSystem::~HealthSystem()
{

}

void HealthSystem::enter()
{

}// end for enter

void HealthSystem::excute(float dt)
{
	//Get all the HealthComponent list
	EntityManager::Component_List* pHealth = EntityManager::getEntityManager()->getComponentList(COMPONENT_HEALTH);

	for(unsigned int entity = 0 ; entity < EntityManager::getEntityManager()->getEntitySize() ;)
	{
		HealthComponent* health = (HealthComponent*)(*pHealth)[entity] ;

		if(health != NULL)
		{
			EntityTypeComponent* pType = (EntityTypeComponent*)EntityManager::getEntityManager()->getComponent(COMPONENT_ENTITY_TYPE, entity);
			if(pType->type == EntityTypeComponent::PLAYER)
			{
				GameInfo::getGameInfo()->CUR_HEALTH_PLAYER = health->health ;
			}

			if(health->health == 0)
			{
				if((EntityManager::getEntityManager()->getEntityFlag(entity) & COMPONENT_ANIMATE) == 0)
				{
					switch(pType->type)
					{
					case EntityTypeComponent::GREEN_CUBE:
					case EntityTypeComponent::RED_CUBE:
						GameInfo::getGameInfo()->CUR_SCORE += 1 ;
						break ;
					
					case EntityTypeComponent::ORANGE_CUBE:
						GameInfo::getGameInfo()->CUR_SCORE += 2 ;
						break ;

					case EntityTypeComponent::PURPLE_CUBE:
						GameInfo::getGameInfo()->CUR_SCORE += 3 ;
						break ;
					}// end switch

					EntityManager::getEntityManager()->removeEntity(entity);
				}
				else
					entity ++ ;
			}// end if
			else
				entity ++ ;
		}// end if
		else
			entity ++ ;
	}// end for
}// end for excute

void HealthSystem::exit()
{

}// end for exit