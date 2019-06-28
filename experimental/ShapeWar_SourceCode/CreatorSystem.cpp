#include"CreatorSystem.h"
#include"EntityCreator.h"
using namespace ShapeWar ;

CreatorSystem::CreatorSystem(int _priority)
	:System(_priority),
	frames(0)
{

}

CreatorSystem::~CreatorSystem()
{

}

void CreatorSystem::enter()
{

}// end for enter

void CreatorSystem::excute(float dt)
{
	frames ++ ;

	static int delta = 0 ;
	delta = frames / 1800 ;

	if(delta >= 30)
		delta = 30 ;

	if(frames % (60 - delta ) == 0)
	{
		int value = rand()%100 ;
		float vy = -60 - (frames / 300.0) * 10 ;

		if(0 <= value&& value < 40)
		{
			EntityCreator::createGreenCube(0, vy);
		}
		else if(40 <= value&& value < 80)
		{
			EntityCreator::createRedCube(0, vy);
		}
		else if(80 <= value && value < 90)
		{
			EntityCreator::createOrangeCube(0, 0.6*vy);
		}
		else if(90 <= value && value<100)
		{
			EntityCreator::createPurpleCube(0,0.4*vy);
		}
	}//end if

}// end for excute

void CreatorSystem::exit()
{

}// end for exit