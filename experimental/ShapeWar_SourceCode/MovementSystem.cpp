#include"MovementSystem.h"
#include"EntityMananger.h"
using namespace ShapeWar ;

MovementSystem::MovementSystem(int _priority)
	:System(_priority)
{

}

MovementSystem::~MovementSystem()
{

}

void MovementSystem::enter()
{

}// end for enter

void MovementSystem::excute(float dt)
{
	unsigned int size = EntityManager::getEntityManager()->getEntitySize();
	for(unsigned int i =  0 ; i < size ; i ++)
	{
		_int64 flag = EntityManager::getEntityManager()->getEntityFlag(i);
		
		if((flag & (COMPONENT_POSITION | COMPONENT_VELOCITY)) == (COMPONENT_POSITION | COMPONENT_VELOCITY))
		{
			PositionComponent* pPos = (PositionComponent*)EntityManager::getEntityManager()->getComponent(COMPONENT_POSITION, i);
			VelocityComponent* pVelocity = (VelocityComponent*)EntityManager::getEntityManager()->getComponent(COMPONENT_VELOCITY, i);

			pPos->x += (1.0 / 60) * pVelocity->vx ;
			pPos->y += (1.0 / 60) * pVelocity->vy ;
		}
	}// end for
}// end for excute

void MovementSystem::exit()
{

}// end for exit



