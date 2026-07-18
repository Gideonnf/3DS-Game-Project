/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:      SystemManager.cpp

 author:	Gideon Francis (100%)

 email:     g.francis@digipen.edu

 brief:     A singleton manager class that controls all systems in the engine. Stores them in a map based on the string representation of the system.
			Also acts as a message dispatcher to systems that subscribe to specific systems to receive messages from them. Also updates all systems when
			an entity has a new component added to map them to their entitiesSet or if an entity was destroyed and needs to be removed

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include "SystemManager.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include <algorithm>

void SystemManager::EntityDestroyed(Entity entity)
{
	for (auto const& pair : mSystems)
	{
		auto& entities = pair.second->mEntitiesSet;
		auto it = std::find(entities.begin(), entities.end(), entity);
		if (it != entities.end())
		{
			pair.second->EntityDestroyed(entity);
			*it = entities.back();
			entities.pop_back();
		}
	}

	EntityManager::GetInstance()->DeleteEntity(entity);
	ComponentManager::GetInstance()->EntityDestroyed(entity);
}

void SystemManager::UpdateSignatures(Entity entity, Signature entitySignature)
{
	for (auto const& pair : mSystems)
	{
		auto const& id = pair.first;
		auto const& system = pair.second;
		auto const& systemSignature = mSystemSignatures[id];

		// Check if the entities signature matches the system's
		if ((entitySignature & systemSignature) == systemSignature) // bit comparison 
		{
			auto& entities = system->mEntitiesSet;
			if (std::find(entities.begin(), entities.end(), entity) == entities.end())
			{
				system->mEntitiesSet.push_back(entity);
				system->EntityAdded(entity);
			}
		}
		else
		{
			auto& entities = system->mEntitiesSet;
			auto it = std::find(entities.begin(), entities.end(), entity);
			if (it != entities.end())
			{
				*it = entities.back();
				entities.pop_back();
				system->EntityRemoved(entity);
			}
		}
	}
}