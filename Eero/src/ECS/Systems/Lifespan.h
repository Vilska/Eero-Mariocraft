#pragma once

#include "ECS/EntityManager.h"

namespace Eero {

	class Lifespan
	{
		friend class Systems;
	private:
		Lifespan(const std::shared_ptr<EntityManager>& entityManager)
			: m_EntityManager(entityManager) {}

		void Update(float deltaTime);
	private:
		std::shared_ptr<EntityManager> m_EntityManager;
	};

}
