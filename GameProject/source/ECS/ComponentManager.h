/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:          ComponentManager.h

 author:        Gideon Francis (100%)

 email:         g.francis@digipen.edu

 brief:         A singleton manager to handle the creation and management of Components. Stores a map of a string representation to the component.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H
#include "Component.h" // ECStypes in component.h
#include <unordered_map>
#include <memory>
#include <any>
#include <cassert>
#include <functional>
#include "ECSTypes.h"
#include "../Singleton.hpp"


	class ComponentManager : public Singleton<ComponentManager>
	{
	private:
		// Map for looking up component IDs by string (used during configuration/deserialization)
		std::unordered_map<std::string, ComponentID> m_ComponentTypes;

		// Array of component wrappers indexed by their compile-time ComponentID
		std::array<std::shared_ptr<IComponent>, MAX_COMPONENTS> m_ComponentMap;

		ComponentID m_NextID = 1;

	public:
		ComponentManager()
		{
			m_ComponentMap.fill(nullptr);
		}

		template<typename T>
		void RegisterComponent(const std::string& name)
		{
			ComponentID id = GetComponentTypeID<T>();

			assert(id < MAX_COMPONENTS && "Exceeded maximum number of components");
			assert(m_ComponentMap[id] == nullptr && "Component has already been registered");

			m_ComponentTypes.insert({ name, id });
			m_ComponentMap[id] = std::make_shared<Component<T>>();
		}

		template<typename T>
		ComponentID GetComponentID()
		{
			return GetComponentTypeID<T>();
		}

        /*!*************************************************************************
        \brief
        	Get the Component ID from a string
        
        \param[in] componentName
        	
        
        \return ComponentID
        	
        ***************************************************************************/
		ComponentID GetComponentID(const std::string& componentName)
		{

			if (m_ComponentTypes[componentName] != 0)
			{
				return m_ComponentTypes[componentName];
			}

			// Only used for checking string since getting typeid of a struct is struct <name>
			// If we want to pass in a string then we have to use string find to see if <name> is in struct <name>
			for (auto const& component : m_ComponentTypes)
			{
				if (component.first.find(componentName) != std::string::npos)
				{
					return component.second;
				}
			}

			assert("String does not match any registered components");
		}

        /*!*************************************************************************
        \brief
        	Add a component to the entity
        
        \tparam T - Type of component
        \param[in] entity - Entity to add to
        	
        
        \param[in] component - Component
        	
        
        ***************************************************************************/
		template<typename T>
		void AddComponent(const Entity& entity, T component)
		{
			GetComponentArray<T>()->InsertComponentData(entity, component);
		}

        /*!*************************************************************************
        \brief
        	Add component to entity
        
        \tparam T  - Type of component
        \param[in] entity - Entity to attach component to
        	
        
        ***************************************************************************/
		template<typename T>
		void AddComponent(const Entity& entity)
		{
			T component{};
			GetComponentArray<T>()->InsertComponentData(entity, component);
		}


        /*!*************************************************************************
        \brief
        	Remove component from an entity
        
        \tparam T 
        \param[in] entity
        	
        
        ***************************************************************************/
		template<typename T>
		void RemoveComponent(const Entity& entity)
		{
			GetComponentArray<T>()->RemoveComponentData(entity);
		}

        /*!*************************************************************************
        \brief
        	Get the Component object
        
        \tparam T 
        \param[in] entity
        	
        
        \return T&
        	
        ***************************************************************************/
		template<typename T>
		T& GetComponent(const Entity& entity)
		{
			return GetComponentArray<T>()->GetComponentData(entity);
		}

        /*!*************************************************************************
        \brief
        	Called when entities are destroyed
        
        \param[in] entity
        	
        
        ***************************************************************************/
		void EntityDestroyed(Entity entity);

        /*!*************************************************************************
        \brief
        	Called when cloning entity
        
        \param[in] entityToClone
        	Entity to clone
        
        \param[in] newEntity
        	Entity to clone into
        
        \param[in] entitySignature
        	Signature to copy
        
        ***************************************************************************/
		void CloneEntity(Entity entityToClone, Entity newEntity, Signature entitySignature);

        /*!*************************************************************************
        \brief
        	For looping through components of an entity
        
        \param[in] func
        	Function
        
        \param[in] entitySignature
        	Entity signature
        
        ***************************************************************************/
		void ForEachComponent(const std::function<void(const std::string&)>& func, const Signature& entitySignature);


		size_t GetComponentCount();

        /*!*************************************************************************
        \brief
        	Check if the entity has the component by calling the HasComponentData
        
        \tparam T 
        \param[in] entity
        	
        
        \return true
        	
        \return false
        	
        ***************************************************************************/
		template<typename T>
		bool HasComponent(Entity entity)
		{
			return GetComponentArray<T>()->HasComponentData(entity);
		}

        /*!*************************************************************************
        \brief
        	Get the Component Array object
		    Used to get the component for inserting in new entity data
        
       \template<typename T>
        	Type of the class
        ***************************************************************************/
		template<typename T>
		std::shared_ptr<Component<T>> GetComponentArray()
		{
			ComponentID id = GetComponentTypeID<T>();
			assert(id < MAX_COMPONENTS && "Component ID out of bounds");
			assert(m_ComponentMap[id] != nullptr && "Component does not exist yet");
			return std::static_pointer_cast<Component<T>>(m_ComponentMap[id]);
		}

	};

#define COMPONENTSYSTEM ComponentManager::GetInstance()
#define REGISTER_COMPONENT(Component) ComponentManager::GetInstance()->RegisterComponent<Component>(#Component)

#endif