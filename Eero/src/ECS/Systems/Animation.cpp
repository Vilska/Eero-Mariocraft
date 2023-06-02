#include "animation.h"
#include "Core/Time.h"
#include "Core/Log.h"

namespace Eero {

	void Animation::Update(float deltaTime)
	{
		for (auto& entity : m_EntityManager->GetEntities())
		{
			// Error checking
			if (!entity->HasComponent<AnimationComponent>())
				continue;

			// Shortcut
			auto& animation = entity->GetComponent<AnimationComponent>();

			// Increase frame count
			animation->FrameCount++;

			// Error checking
			if (animation->FrameCount != animation->Speed && animation->FrameCount != 1)
				continue;

			animation->FrameCount = 1;

			// Change texture coords
			float posX = animation->TexRectPos.x;

			// Add basis if direction is left
			if (animation->Direction == -1)
				posX += animation->TexRectSize.x;

			Vec2 actualPos = { posX + (animation->TexRectSize.x * animation->sXIndex), animation->TexRectPos.y + (animation->TexRectSize.y * animation->sYIndex) };
			animation->Sprite->setTextureRect(sf::IntRect(actualPos.x, actualPos.y, animation->Direction * animation->TexRectSize.x, animation->TexRectSize.y));

			animation->sXIndex++;

			if (animation->sXIndex == animation->States.x)
			{
				animation->sYIndex++;

				if (animation->sYIndex == animation->States.y)
				{
					animation->sYIndex = 0;
				}

				animation->sXIndex = 0;
			}
		}
	}

}