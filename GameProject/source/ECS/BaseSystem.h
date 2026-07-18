#ifndef BASE_SYSTEM_H
#define BASE_SYSTEM_H
#include <set>
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <chrono>
#include <typeinfo>
#include <assert.h>
#include "ECSTypes.h"

class BaseSystem
{
public:
	// contain a set that holds what entities it holds
	std::set<Entity> mEntitiesSet;
	// Keep track of the system's signature
	Signature mSignature;

	void PrintEntities(std::string systemName)
	{
		std::cout << "Entities in "<< systemName << " : " << mEntitiesSet.size() << std::endl;
	}

	virtual void EntityDestroyed(Entity id) {};

	virtual void EntityAdded(Entity id) {};

	virtual void EntityRemoved(Entity id) {};

	virtual void OnUpdate() {}; // New virtual method for actual update logic

	void Update() {
		OnUpdate();
	}

	// Keep track of time spent within system
	std::chrono::milliseconds mMilliseconds{};
	// Holds a vector of what components it uses.
	std::vector<const char*> componentNames{};
};


#endif