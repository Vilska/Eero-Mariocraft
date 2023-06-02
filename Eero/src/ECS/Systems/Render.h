#pragma once

#include "ECS/EntityManager.h"
#include "Window/Window.h"

namespace Eero {

	class Render
	{
		friend class Systems;
	private:
		Render(const std::shared_ptr<Window>& window, const std::shared_ptr<EntityManager>& entityManager)
			: m_Window(window), m_EntityManager(entityManager) {}

		void Update(float deltaTime);
	private:
		std::shared_ptr<Window> m_Window;
		std::shared_ptr<EntityManager> m_EntityManager;
	};

}
