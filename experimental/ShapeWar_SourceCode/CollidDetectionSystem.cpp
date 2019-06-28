#include"CollidDetectionSystem.h"
#include"EntityMananger.h"
#include"AudioSystem.h"
using namespace ShapeWar ;

CollidDetectionSystem::CollidDetectionSystem(int _priority)
	:System(_priority)
{

}

CollidDetectionSystem::~CollidDetectionSystem()
{

}

void CollidDetectionSystem::enter()
{

}// end for enter

void CollidDetectionSystem::excute(float dt)
{
	//Get all PositionComponent list
	EntityManager::Component_List* pPos = EntityManager::getEntityManager()->getComponentList(COMPONENT_POSITION);

	//Get all the CollidableComponent list
	EntityManager::Component_List* pCollid = EntityManager::getEntityManager()->getComponentList(COMPONENT_COLLID);

	//Get all the EntityTypeComponent list
	EntityManager::Component_List* pType = EntityManager::getEntityManager()->getComponentList(COMPONENT_ENTITY_TYPE);

	//Get all the HealthComponent list
	EntityManager::Component_List* pHealth = EntityManager::getEntityManager()->getComponentList(COMPONENT_HEALTH);

	unsigned int size = EntityManager::getEntityManager()->getEntitySize();

	//Find all sphere ball
	std::vector<unsigned int> index_array ;
	for(unsigned int i = 0 ; i < size ; i ++)
	{
		if(((EntityTypeComponent*)(*pType)[i])->type == EntityTypeComponent::SPHERE_BALL)
		{
			if((EntityManager::getEntityManager()->getEntityFlag(i) & COMPONENT_VELOCITY) == COMPONENT_VELOCITY)
			{
				index_array.push_back(i);
			}// end if
		}// end if
	}// end for

	for(unsigned int i = 0 ; i < index_array.size() ; i ++)
	{
		CollidableComponent* collidAreaA = ((CollidableComponent*)((*pCollid)[index_array[i]])) ;
		PositionComponent* posA = ((PositionComponent*)((*pPos)[index_array[i]])) ;
		collidAreaA->min_x = posA->x - 16 ;
		collidAreaA->min_y = posA->y - 16 ;
		collidAreaA->max_x = posA->x + 16 ;
		collidAreaA->max_y = posA->y + 16 ;

		size = EntityManager::getEntityManager()->getEntitySize();
		for(unsigned int j = 0 ; j < size ; j ++)
		{
			if((EntityManager::getEntityManager()->getEntityFlag(j) & COMPONENT_COLLID) == COMPONENT_COLLID &&
				((EntityTypeComponent*)(*pType)[j])->type != EntityTypeComponent::SPHERE_BALL)
			{
				CollidableComponent* collidAreaB = ((CollidableComponent*)((*pCollid)[j])) ;
				PositionComponent* posB = ((PositionComponent*)((*pPos)[j])) ;
				collidAreaB->min_x = posB->x - 16 ;
				collidAreaB->min_y = posB->y - 16 ;
				collidAreaB->max_x = posB->x + 16 ;
				collidAreaB->max_y = posB->y + 16 ;

				if(collidAreaA->min_x > collidAreaB->max_x
					||collidAreaA->max_x < collidAreaB->min_x) continue ;
				if(collidAreaA->min_y > collidAreaB->max_y ||
					collidAreaA->max_y < collidAreaB->min_y) continue ;

				HealthComponent* cube = (HealthComponent*)(*pHealth)[j] ;
				cube->health -- ;

				if(cube->health == 0)
				{
					AnimateComponent* pAnimate = new AnimateComponent();
					pAnimate->animate = new CCAnimate();
					
					CCAnimation* pAnimation =  CCAnimation::create();
					
					for(int i = 0 ; i < 10 ; i ++)
					{
						char buffer[32] ;
						sprintf(buffer,"Explosion000%d.png",i);
						pAnimation->addSpriteFrameWithFileName(buffer);
					}// end for

					pAnimation->setDelayPerUnit(1.0/10);
					pAnimate->animate->initWithAnimation(pAnimation);
					pAnimate->frames = 60 ;

					//Add the Animate Component to the entity
					EntityManager::getEntityManager()->addComponent(pAnimate, COMPONENT_ANIMATE, j);

					//Remove the CollidDetection Component
					EntityManager::getEntityManager()->removeComponent(COMPONENT_COLLID, j);

					//Remove the Velocity Component
					EntityManager::getEntityManager()->removeComponent(COMPONENT_VELOCITY, j);

				}// end if

				HealthComponent* ball = (HealthComponent*)(*pHealth)[index_array[i]] ;
				ball->health -- ;

				//Play hurt effect
				AudioSystem::sharedAudioSystem()->playSound("Hurt.wav");

				break ;
			}// end if
		}// end for cube
	}// end for sphere ball
}// end for excute

void CollidDetectionSystem::exit()
{

}// end for exit