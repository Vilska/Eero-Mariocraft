#include "Render.h"
#include "Core/Log.h"

namespace Eero {

	void Render::Update(float deltaTime)
	{
		for (auto& entity : m_EntityManager->GetEntities())
		{
			if (entity->HasComponent<SpriteComponent>() && entity->HasComponent<TransformComponent>())
			{
				m_Window->Draw(*entity->GetComponent<SpriteComponent>()->Sprite);
				entity->GetComponent<SpriteComponent>()->Sprite->setPosition(entity->GetComponent<TransformComponent>()->Pos.x, entity->GetComponent<TransformComponent>()->Pos.y);
			}

			if (entity->HasComponent<TextComponent>())
			{
				m_Window->Draw(entity->GetComponent<TextComponent>()->Text);
			}
		}
	}

}