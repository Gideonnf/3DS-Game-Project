/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:		SystemManager.h

 author:	Gideon Francis (100%)

 email:     g.francis@digipen.edu

 brief:     A singleton manager class that controls all systems in the engine. Stores them in a map based on the string representation of the system.
			Also acts as a message dispatcher to systems that subscribe to specific systems to receive messages from them. Also updates all systems when
			an entity has a new component added to map them to their entitiesSet or if an entity was destroyed and needs to be removed

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include "BaseSystem.h"
#include <unordered_map>
#include <memory>
#include <vector>
#include <string>
#include <cassert>
#include "../Singleton.hpp"
#include "ComponentManager.h"

class SystemManager : public Singleton<SystemManager>
{
private:
	std::unordered_map<SystemID, Signature> mSystemSignatures{};
	std::unordered_map<SystemID, std::shared_ptr<BaseSystem>> mSystems{};

public:
	template<typename T>
	std::shared_ptr<T> RegisterSystem()
	{
		SystemID id = GetSystemTypeID<T>();
		auto system = std::make_shared<T>();
		mSystems.insert({ id, system });
		return system;
	}

	template<typename T>
	void SetSignature(Signature sig)
	{
		SystemID id = GetSystemTypeID<T>();
		mSystemSignatures.insert({ id, sig });
	}

	template<typename T>
	void SetSignature(std::vector<const char*> componentList)
	{
		SystemID id = GetSystemTypeID<T>();

		Signature signature;
		for (auto const& component : componentList)
		{
			mSystems[id]->componentNames.push_back(component);
			signature.set(ComponentManager::GetInstance()->GetComponentID(component), true);
		}

		mSystems[id]->mSignature = signature;
		mSystemSignatures.insert({ id, signature });
	}

	void EntityDestroyed(Entity entity);

	void UpdateSignatures(Entity entity, Signature entitySignature);

	template <typename T>
	std::shared_ptr<T> GetSystem()
	{
		SystemID id = GetSystemTypeID<T>();
		if (mSystems.count(id) != 0)
		{
			return std::static_pointer_cast<T>(mSystems[id]);
		}

		assert(false && "System does not exist yet");
		return nullptr;
	}
};

#define SYSTEM SystemManager::GetInstance()
#define REGISTER_SYSTEM(system) SystemManager::GetInstance()->RegisterSystem<system>();

#endif