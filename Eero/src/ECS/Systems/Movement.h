#pragma once

#include "ECS/EntityManager.h"

namespace Eero {

	class Movement
	{
		friend class Systems;
	private:
		Movement(const std::shared_ptr<EntityManager>& entityManager)
			: m_EntityManager(entityManager) {}

		void Update(float deltaTime);
	private:
		std::shared_ptr<EntityManager> m_EntityManager;
	};
}
