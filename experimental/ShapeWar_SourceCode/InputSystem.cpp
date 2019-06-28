#include "InputSystem.h"
#include "EntityMananger.h"
#include "EntityCreator.h"
#include "AudioSystem.h"
using namespace ShapeWar ;

InputSystem::InputSystem(int _priority)
	:System(_priority)
{

}

InputSystem::~InputSystem()
{

}

void InputSystem::enter()
{

}// end for enter

void InputSystem::excute(float dt)
{
	//Get the Component list
	EntityManager::Component_List* pPos = EntityManager::getEntityManager()->getComponentList(COMPONENT_POSITION);
	EntityManager::Component_List* pType = EntityManager::getEntityManager()->getComponentList(COMPONENT_ENTITY_TYPE);

	//Find the player and the un-shooted ball
	unsigned int size = EntityManager::getEntityManager()->getEntitySize();

	int player = -1 , ball = -1 ;
	for(unsigned int i = 0 ; i < size ; i ++)
	{
		unsigned int type = ((EntityTypeComponent*)(*pType)[i])->type ;
		if(type == EntityTypeComponent::PLAYER)
		{
			player = i ;
		}// end if

		if(type == EntityTypeComponent::SPHERE_BALL)
		{
			_int64 flag = EntityManager::getEntityManager()->getEntityFlag(i);
			if((flag & COMPONENT_VELOCITY) == 0)
			{
				ball = i ;
			} // end if
		}// end if

		if(player != -1 && ball != -1)
			break ;
	}// end for

	PositionComponent* pPlayer_Pos = NULL ;
	PositionComponent* pBall_Pos = NULL ;

	if(player != -1)
		pPlayer_Pos = (PositionComponent*)(*pPos)[player] ;
	if(ball != -1)
		pBall_Pos = (PositionComponent*)(*pPos)[ball] ;

	if(GetKeyState(VK_RIGHT) & 0x8000)
	{
		if(pPlayer_Pos != NULL)
		{
			pPlayer_Pos->x += 5 ;
			if(pPlayer_Pos->x >= 320 - 22)
				pPlayer_Pos->x = 320 - 22 ;

			if(pBall_Pos != NULL)
				pBall_Pos->x = pPlayer_Pos->x ;
		}
	}else if(GetKeyState(VK_LEFT)&0x8000)
	{
		if(pPlayer_Pos != NULL)
		{
			pPlayer_Pos->x -= 5 ;
			if(pPlayer_Pos->x <= 22)
				pPlayer_Pos->x = 22 ;

			if(pBall_Pos != NULL)
				pBall_Pos->x = pPlayer_Pos->x ;
		}
	}

	static int nFrame = 0 ;
	if((GetKeyState(VK_SPACE)& 0x8000) && (nFrame >= 15))
	{
		VelocityComponent* pVelocity = new VelocityComponent();
		pVelocity->vx = 0 ;
		pVelocity->vy = 600 ;
		EntityManager::getEntityManager()->addComponent(pVelocity, COMPONENT_VELOCITY, ball);

		//Create another ball
		EntityCreator::createSphereBall(pPlayer_Pos->x, pPlayer_Pos->y);

		//Player Effect
		AudioSystem::sharedAudioSystem()->playSound("Shoot.wav");
		nFrame = 0 ;
	}

	nFrame ++ ;

}// end for excute

void InputSystem::exit()
{

}// end for exit