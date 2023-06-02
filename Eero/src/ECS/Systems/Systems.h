#pragma once

#include "Collision.h"
#include "Movement.h"
#include "Render.h"
#include "Lifespan.h"
#include "Animation.h"

namespace Eero {

	// Systems
	class Systems
	{
	public:
		Systems() = default;
		Systems(std::shared_ptr<Window>& window, std::shared_ptr<EntityManager>& entityManager);

		void Run(float deltaTime);
		
		std::shared_ptr<Collision>& GetCollision() { return m_Collision; }
	private:
		std::shared_ptr<Collision> m_Collision;
		std::shared_ptr<Movement> m_Movement;
		std::shared_ptr<Render> m_Render;
		std::shared_ptr<Lifespan> m_Lifespan;
		std::shared_ptr<Animation> m_Animation;
	};

}
