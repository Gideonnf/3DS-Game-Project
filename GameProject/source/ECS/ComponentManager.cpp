/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			ComponentManager.cpp

 author:		Gideon Francis (100%)

 email:			g.francis@digipen.edu

 brief:			A singleton manager to handle the creation and management of Components. Stores a map of a string representation to the component.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include "ComponentManager.h"

	void ComponentManager::EntityDestroyed(Entity entity)
	{
		for (auto const& compArray : m_ComponentMap)
		{
			if (compArray)
			{
				compArray->EntityDestroyed(entity);
			}
		}
	}

	void ComponentManager::CloneEntity(Entity entityToClone, Entity newEntity, Signature entitySignature)
	{
		for (auto const& component : m_ComponentTypes)
		{
			Signature componentSignature;
			componentSignature.set(component.second, true);

			if ((entitySignature & componentSignature) == componentSignature)
			{
				m_ComponentMap[component.second]->CloneComponentData(entityToClone, newEntity);
			}
		}
	}

	void ComponentManager::ForEachComponent(const std::function<void(const std::string&)>& func, const Signature& entitySignature)
	{
		for (auto const& component : m_ComponentTypes)
		{
			Signature componentSignature;
			// Set that component signature to true
			componentSignature.set(component.second, true);
			// Check if the entity's signature has this component

			// It has that component
			if ((entitySignature & componentSignature) == componentSignature)
			{
				func(component.first);
			}
		}
	}

	size_t ComponentManager::GetComponentCount()
	{
		return m_ComponentTypes.size();
	}