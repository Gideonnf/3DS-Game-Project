/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			Component.h

 author:		Gideon Francis (100%)

 email:			g.francis@digipen.edu

 brief:			The component template class that allows for any kind of component creation. Uses two maps, Entity ID to Component and Component to Entity ID.
				Components are stored in a vector that reserves a size based on the max entities that the system allows to allow for contiguous memory of components

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#ifndef COMPONENT_H
#define COMPONENT_H

#include "ECSTypes.h"
#include <unordered_map>
#include <array>
#include <vector>
#include <string>
#include <cassert>
#include <typeinfo>

// Interface for components so that component manager can notify components if an entity is destroyed
class IComponent
{
public:
	virtual ~IComponent() {};
	virtual void EntityDestroyed(Entity entity) = 0;
	virtual void CloneComponentData(Entity, Entity) = 0;
};

template <typename T>
class Component : public IComponent
{
private:
	// Initialize an array of components that matches the max number of entities
	// So each entity will match it's own component data
	//std::array<T, MAX_ENTITIES> m_ComponentArray;
	std::vector<T> m_ComponentArray;

	// Map for an entity ID to a component
	//std::unordered_map<Entity, unsigned int> m_EntityToComponent;
	std::vector<Entity> m_ComponentToEntity;

	// Map a component to an entity
	//std::unordered_map<unsigned int, Entity> m_ComponentToEntity;
	std::array<unsigned int, MAX_ENTITIES> m_EntityToComponent;

	// Keep track of the active components
	//unsigned int m_Size{};

	// NOTE:
	/*
		Possible to initialize a queue based on the number of components
		when an entity wants to add a component, itll take the first component available
		if an entity 
	*/

public:
	Component()
	{
		// Reserve memory for the component array to allow for continous memory
		m_ComponentArray.reserve(MAX_ENTITIES);
		m_ComponentToEntity.reserve(MAX_ENTITIES);
		m_EntityToComponent.fill(MAX_ENTITIES);
	}

	/*!*************************************************************************
	\brief
			Insert component data into the array
	
	\param[in] entity
		Entity to pair to
	
	\param[in] component
		Component to insert
	
	***************************************************************************/
	void InsertComponentData(Entity entity, T component)
	{
		if (m_ComponentArray.size() > MAX_ENTITIES)
		{
			assert("Too many entities have been added.");
		}

		// use current active size to get the next component id
		unsigned int componentIndex = (unsigned int)m_ComponentArray.size();

		// Set the map variables
		// m_EntityToComponent[entity] = componentIndex;
		// m_ComponentToEntity[componentIndex] = entity;
		m_EntityToComponent[entity] = componentIndex;
		m_ComponentToEntity.push_back(entity);

		// Store the component data into the array
		//m_ComponentArray[componentIndex] = component;
		m_ComponentArray.push_back(component);

		// increment size to ensure its always the latest number
		//m_Size++;
	}

	/*!*************************************************************************
	\brief
		remove component data related to entity id
	
	\param[in] entity
		Entity to remove component from
	
	***************************************************************************/
	void RemoveComponentData(Entity entity)
	{
		assert(m_EntityToComponent[entity] != MAX_ENTITIES && "Entity does not contain this component");

		// Get the last active index and the entity to delete index
		unsigned int lastValidIndex = (unsigned int)m_ComponentArray.size() - 1;
		// get the component index related to the entity thats being removed
		unsigned int componentIndex = m_EntityToComponent[entity];

		
		// Swap component data from within component array
		// Swap the values in the component array
		m_ComponentArray[componentIndex] = m_ComponentArray[lastValidIndex];
		// pop back the last element after swapping to maintain size
		m_ComponentArray.pop_back();

		// Update dense-to-entity mapping
		Entity lastValidEntity = m_ComponentToEntity[lastValidIndex];
		m_ComponentToEntity[componentIndex] = lastValidEntity;
		m_ComponentToEntity.pop_back();

		// Update sparse mapping
		m_EntityToComponent[lastValidEntity] = componentIndex;
		m_EntityToComponent[entity] = MAX_ENTITIES; // Reset

		// Decrement size as the last valid index is now free to be used
		//m_Size--;
	}

	/*!*************************************************************************
	\brief
		Get the Component Data object
	
	\param[in] entity to get from
		
	
	\return T&
		
	***************************************************************************/
	T& GetComponentData(Entity entity)
	{
		assert(m_EntityToComponent[entity] != MAX_ENTITIES && "Entity does not contain this component");

		return m_ComponentArray[m_EntityToComponent[entity]];
	}

	/*!*************************************************************************
	\brief
		Check if entity has that component data
	
	\param[in] entity
		Entity to check
	
	\return true if have
		
	\return false if dont have
		
	***************************************************************************/
	bool HasComponentData(Entity entity)
	{
		// If component has this entity, check if sparse index is valid
		return m_EntityToComponent[entity] != MAX_ENTITIES;
	}

	/*!*************************************************************************
	\brief
		Clone component data related to the entity
	
	\param[in] entityToClone
		Entity to clone data from
	
	\param[in] newEntity
		Entity to clone into
	
	***************************************************************************/
	void CloneComponentData(Entity entityToClone, Entity newEntity)
	{
		T componentData = GetComponentData(entityToClone);
		InsertComponentData(newEntity, componentData);
	}

	/*!*************************************************************************
	\brief
		When an entity is destroyed, call this
	
	\param[in] entity
		
	
	***************************************************************************/
	void EntityDestroyed(Entity entity) override
	{
		// If the entity exist within this component
		if (HasComponentData(entity))
		{
			// Remove the data
			RemoveComponentData(entity);
		}
	}
};

#endif
