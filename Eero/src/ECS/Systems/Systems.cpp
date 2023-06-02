#include "Systems.h"

namespace Eero {

	// Systems
	Systems::Systems(std::shared_ptr<Window>& window, std::shared_ptr<EntityManager>& entityManager)
	{
		m_Collision = std::shared_ptr<Collision>(new Collision(entityManager));
		m_Movement = std::shared_ptr<Movement>(new Movement(entityManager));
		m_Render = std::shared_ptr<Render>(new Render(window, entityManager));
		m_Lifespan = std::shared_ptr<Lifespan>(new Lifespan(entityManager));
		m_Animation = std::shared_ptr<Animation>(new Animation(entityManager));
	}

	void Systems::Run(float deltaTime)
	{
		m_Movement->Update(deltaTime);
		m_Render->Update(deltaTime);
		m_Lifespan->Update(deltaTime);
		m_Animation->Update(deltaTime);
	}

}
