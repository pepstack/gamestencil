//------------------------------------------------------------------
// declaration	: Copyright (c), by XJ , 2014 . All right reserved .
// brief		: This file will define the Entity of the Entity-Componet-
//                System and the entity manager.
// author		: XJ
// date			: 2014 / 6 / 8
// version		: 1.0
//-------------------------------------------------------------------
#pragma once

#include<vector>
#include"Component.h"
using namespace std ;

namespace ShapeWar
{
/**
* Define the EntityManager
*/
class EntityManager
{
private:
	EntityManager();
	~EntityManager();

	/** Singleton getter*/
public:
	static EntityManager* getEntityManager();

	/** Core method */
public:
	/**
	* Create an empty entity
	*/
	_int64 createEntity() ;

	/**
	* Remove an entity
	*/
	void removeEntity(_int64 entity);

	/**
	* Register component
	* brief	: This method will make the entity manager to alloc the memory to store
	*         the registed componet.If you want to use one componet in the ECS , you
	*         must registed it at the start time.
	*/
	void registComponents(_int64 component_size);

	/**
	* Add an component to the entity
	*/
	void addComponent(Component* component, _int64 component_type, _int64 entity);

	/**
	* Remove an component of the entity
	*/
	void removeComponent(_int64 component_type, _int64 entity);

	/**
	* Get component list
	*/
	std::vector<Component*>* getComponentList(_int64 component_type) const ;

	/**
	* Get the specificed component of the entity
	*/
	Component* getComponent(_int64 component_type, _int64 entity);

	/**
	* Get entity flag
	*/
	_int64 getEntityFlag(_int64 entity) const ;

	/**
	* Set entity flag
	*/
	void setEntityFlag(_int64 entity, _int64 entity_type);

	/**
	* Get the entity size
	*/
	unsigned int getEntitySize() const ;

	/**
	* Define the Component_List
	*/
	typedef std::vector<Component*> Component_List;

private:
	/**
	* Destroy all the component
	*/
	void _destroy();

private:
	std::vector<_int64> m_EntityFlagArray ;							//Contain the Entity flag 
	std::vector<Component_List>  m_ComponentContainer ;				//Contain all the entity
};

};
