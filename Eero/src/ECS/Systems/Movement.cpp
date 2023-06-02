#include "Movement.h"

namespace Eero {

	void Movement::Update(float deltaTime)
	{
		for (auto& entity : m_EntityManager->GetEntities())
		{
			if (entity->HasComponent<TransformComponent>())
			{
				auto& tCom = entity->GetComponent<TransformComponent>();
				auto& position = tCom->Pos;
				auto& velocity = tCom->Velocity;
				auto& gravity = tCom->Gravity;

				// Save the previous position
				tCom->PrevPos = position;

				velocity += gravity * deltaTime;
				position += velocity * deltaTime;

				// Limit gravity
				if (velocity.y > tCom->MaxSpeed)
					velocity.y = tCom->MaxSpeed;
			}
		}
	}

}