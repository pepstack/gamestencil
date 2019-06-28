#include"SystemManager.h"
using namespace ShapeWar ;

System::System(int _priority)
	:priority(_priority)
{

}

System::~System()
{

}

SystemManager::SystemManager()
	:bPaused(false)
{
	
}

SystemManager::~SystemManager()
{
	_destroy();
}

SystemManager* SystemManager::getSystemManager()
{
	static SystemManager manager ;
	return &manager ;
}// end for getSystemManager

void SystemManager::addSystem(System* system)
{
	system->enter();

	int size = system_list.size();

	//The first two , we directly insert into the list
	if(size == 0)
	{
		system_list.push_back(system);
		return ;
	}else if(size == 1)
	{
		if(system_list[0]->priority >= system->priority)
		{
			system_list.push_back(system);
			return ;
		}
		else
		{
			system_list.insert(system_list.begin(), system);
			return ;
		}
	}

	//The new system's priority is bigger than all the systems
	if(system->priority >= system_list[0]->priority)
	{
		system_list.insert(system_list.begin(), system);
		return ;
	}
	//The new system's priority is samller than all the systems
	else if(system->priority <= system_list[size - 1]->priority)
	{
		system_list.push_back(system);
		return ;
	}
	
	//Use binary search method to find the system's position which is
	//between the biggest and samllest prority
	int start = 0 ;
	int end = size - 1 ;
	while(true)
	{
		int mid = (start + end) / 2 ;
		if(system_list[mid]->priority >= system->priority &&
			system_list[mid + 1]->priority <= system->priority)
		{
			//Insert into this point
			system_list.insert(system_list.begin() + mid + 1, system);
			break ;
		}
		else if(system_list[mid]->priority < system->priority)
		{
			end = mid ;
		}
		else if(system_list[mid + 1]->priority > system->priority)
		{
			start = mid + 1 ;
		}
	}// end while
}// end for add System

void SystemManager::update(float dt)
{
	if(bPaused == true)
		return ;

	//Excute all the system
	for(int i = 0  ; i < system_list.size() ; i ++)
	{
		system_list[i]->excute(dt);
	}// end for
}// end for update

void SystemManager::pause()
{
	bPaused = true ;
}// end for pause

void SystemManager::resume()
{
	bPaused = false ;
}// end for resume

void SystemManager::_destroy()
{
	for(int i = 0 ; i < system_list.size(); i ++)
	{
		System* system = system_list[i] ;
		system->exit();
		if(system != NULL)
			delete system ;
		system = NULL ;
	}// end for 

	system_list.clear();
}// end for _destory