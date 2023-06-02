#pragma once

#include <tuple>
#include <algorithm>

#include "Components.h"

namespace Eero {

	class Entity
	{
		friend class EntityManager;
	public:
		bool IsActive() const { return m_Active; }
		const std::string& GetTag() const { return m_Tag; }
		const size_t GetIdentifier() const { return m_ID; }
		void Destroy() { m_Active = false; }

		template<typename T>
		bool HasComponent() const
		{
			if (GetComponent<T>() == nullptr)
				return false;

			return true;
		}

		template<typename T, typename... Args>
		void AddComponent(Args&&... args)
		{
			// Overwrite already existing one
			if (HasComponent<T>())
				RemoveComponent<T>();

			auto component = std::make_shared<T>(*this, std::forward<Args>(args)...);
			m_Components.push_back(component);
		}

		template<typename T>
		const std::shared_ptr<T>& GetComponent() const
		{
			for (auto& component : m_Components)
			{
				std::shared_ptr<T> derived = std::dynamic_pointer_cast<T>(component);

				if (derived != nullptr)
					return derived;
			}

			return nullptr;
		}

		template<typename T>
		void RemoveComponent()
		{
			auto component = GetComponent<T>();

			if (component == nullptr)
				return;

			m_Components.erase(std::remove(m_Components.begin(), m_Components.end(), component), m_Components.end());
		}

	private:
		Entity(const size_t id, const std::string& tag) 
			: m_ID(id), m_Tag(tag) {}
	private:
		bool m_Active = true;
		size_t m_ID = 0;
		std::string m_Tag = "Default";

		std::vector<std::shared_ptr<BaseComponent>> m_Components;
		//Entity* m_Instance;
	};

}
