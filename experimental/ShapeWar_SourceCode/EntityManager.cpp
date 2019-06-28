#include"EntityMananger.h"
#include"Helper.h"
using namespace ShapeWar ;

EntityManager::EntityManager()
{

}

EntityManager::~EntityManager()
{
	_destroy();
}

EntityManager* EntityManager::getEntityManager()
{
	static EntityManager manager ;
	return &manager ;
}// end for getEntityManager

_int64 EntityManager::createEntity()
{
	_int64 entity = m_EntityFlagArray.size();
	m_EntityFlagArray.push_back(COMPONENT_NONE);

	_int64 size = m_ComponentContainer.size();
	for(int i = 0 ; i < size ; i ++)
	{
		m_ComponentContainer[i].push_back(NULL);
	}// end for

	return entity ;
}// end for createEntity

void EntityManager::removeEntity(_int64 entity)
{
	assert(entity >= 0 && "Entity must be bigger or equal than 0");
	_int64 size = m_EntityFlagArray.size();
	assert(entity < size && "Entity must be smaller than the size, so that we can remove the entity");
	
	//Iterate all the component container and remove the specificed entity
	for(int i = 0 ; i < m_ComponentContainer.size() ; i ++)
	{
		//Get the iterator of the entity
		std::vector<Component*>::iterator it = m_ComponentContainer[i].begin() + entity ;

		//Delete the component
		Component* pComponent = *it ;
		if(pComponent != NULL)
			delete pComponent ;

		//Remove from the component container
		m_ComponentContainer[i].erase(it);
	}// end for 

	//Remove the flag
	m_EntityFlagArray.erase(m_EntityFlagArray.begin() + entity);
}// end for removeEntity

void EntityManager::registComponents(_int64 component_size)
{
	for(int i = 0 ; i < component_size ; i ++)
	{
		Component_List list ;
		m_ComponentContainer.push_back(list);
	}
}// end for registComponent

void EntityManager::addComponent(Component* component, _int64 component_type, _int64 entity)
{
	assert(entity >= 0 && "Entity must be bigger or equal than 0");
	_int64 size = m_EntityFlagArray.size();
	assert(entity < size && "Entity must be smaller than the size, so that we can remove the entity");

	//Get the component mask of the entity
	_int64 mask = m_EntityFlagArray[entity] ;

	//Check if there is a component that already match the request component type
	if((mask & component_type) == component_type)
		return ;

	m_EntityFlagArray[entity] |= component_type ;

	//We get the component container that contain such component type
	_int64 index = getbitone64(component_type) - 1;
	m_ComponentContainer[index][entity] = component;
}// end for addComponent

void EntityManager::removeComponent(_int64 component_type, _int64 entity)
{
	assert(entity >= 0 && "Entity must be bigger or equal than 0");
	_int64 size = m_EntityFlagArray.size();
	assert(entity < size && "Entity must be smaller than the size, so that we can remove the entity");

	//Get the component mask of the entity
	_int64 mask = m_EntityFlagArray[entity] ;

	//Check if there is a component that already match the request component type
	if((mask & component_type) == 0)
		return ;

	m_EntityFlagArray[entity] &= ~((unsigned _int64)component_type);

	//We get the component container that contain such component type
	_int64 index = getbitone64(component_type) - 1;
	Component * pComponent = m_ComponentContainer[index][entity] ;

	if(pComponent != NULL)
		delete pComponent ;

	m_ComponentContainer[index][entity] = NULL ;
}// end for removeComponent

std::vector<Component*>* EntityManager::getComponentList(_int64 component_type) const
{
	//We get the component container that contain such component type
	_int64 index = getbitone64(component_type) - 1;

	return (std::vector<Component*>*)&m_ComponentContainer[index] ;
}// end for getComponentList

Component* EntityManager::getComponent(_int64 component_type, _int64 entity)
{
	assert(entity >= 0 && "Entity must be bigger or equal than 0");
	_int64 size = m_EntityFlagArray.size();
	assert(entity < size && "Entity must be smaller than the size, so that we can remove the entity");

	//Get the entity flag
	_int64 flag = m_EntityFlagArray[entity];

	//Check if the flag has the specificed component_type
	if((flag & component_type) == component_type)
	{
		int index = getbitone64(component_type) - 1;
		return m_ComponentContainer[index][entity] ;
	}
	else
		return NULL ;

	return NULL ;
}// end for getComponent

_int64 EntityManager::getEntityFlag(_int64 entity) const
{
	assert(entity >= 0 && "Entity must be bigger or equal than 0");
	_int64 size = m_EntityFlagArray.size();
	assert(entity < size && "Entity must be smaller than the size, so that we can remove the entity");

	return m_EntityFlagArray[entity] ;
}// end for getEntityFlag

void EntityManager::setEntityFlag(_int64 entity, _int64 entity_type)
{
	assert(entity >= 0 && "Entity must be bigger or equal than 0");
	_int64 size = m_EntityFlagArray.size();
	assert(entity < size && "Entity must be smaller than the size, so that we can remove the entity");

	m_EntityFlagArray[entity] = entity_type ;
}// end for setEntityFlag

unsigned int EntityManager::getEntitySize() const
{
	return m_EntityFlagArray.size(); 
}// end for getEntitySize

void EntityManager::_destroy()
{
	for(int i = 0 ; i < m_ComponentContainer.size() ; i ++)
	{
		for(int j = 0 ; j < m_ComponentContainer[i].size() ; j ++)
		{
			Component* pComponent = m_ComponentContainer[i][j] ;
			if(pComponent != NULL)
				delete pComponent ;
			pComponent = NULL ;
		}// end for

		m_ComponentContainer[i].clear();
	}// end for

	m_ComponentContainer.clear();
}// end for _destroy