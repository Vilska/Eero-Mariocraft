#include "Lifespan.h"

namespace Eero {

	void Lifespan::Update(float deltaTime)
	{
		for (auto& entity : m_EntityManager->GetEntities())
		{
			if (!entity->HasComponent<SpriteComponent>() || !entity->HasComponent<LifespanComponent>())
				continue;

			int& totalTime = entity->GetComponent<LifespanComponent>()->TotalTime;
			int actionTime = entity->GetComponent<LifespanComponent>()->ActionTime;

			if (totalTime < 0 && totalTime > actionTime)
				continue;

			if (totalTime == 0)
			{
				entity->Destroy();
				continue;
			}

			switch (entity->GetComponent<LifespanComponent>()->Effect)
			{
				case LifespanComponent::EffectTypes::Disappear:
				{
					break;
				}

				case LifespanComponent::EffectTypes::Fade:
				{
					// FillColor
					auto& sprite = entity->GetComponent<SpriteComponent>()->Sprite;
					auto& fillColor = sprite->getColor();
					int fillAlpha = fillColor.a - (fillColor.a / totalTime);

					sprite->setColor(sf::Color(fillColor.r, fillColor.g, fillColor.b, fillAlpha));

					break;
				}

				case LifespanComponent::EffectTypes::Blink:
				{
					//int halfTime = Time::Seconds(1) / 2;
					//int modulo = actionTime % halfTime;

					//// FillColor
					//auto& fillColor = entity->cShape->Circle.getFillColor();
					//static int originalFillAlpha = fillColor.a;
					//int fillAlpha = fillColor.a;

					//// OutlineColor
					//auto& outlineColor = entity->cShape->Circle.getOutlineColor();
					//static int originalOutlineAlpha = outlineColor.a;
					//int outlineAlpha = outlineColor.a;

					//// Adjustable with seconds, on every second entity fades out and fades in
					//static int timer = Time::Seconds(1);

					//if (timer > halfTime && timer <= timer)
					//{
					//	fillAlpha = (originalFillAlpha * (timer - halfTime) / halfTime);
					//	outlineAlpha = (originalOutlineAlpha * (timer - halfTime) / halfTime);
					//}
					//else if (timer > 0 && timer <= halfTime)
					//{
					//	fillAlpha += originalFillAlpha / halfTime;
					//	outlineAlpha += originalOutlineAlpha / halfTime;
					//}
					//else
					//{
					//	timer = Time::Seconds(1);
					//}
					//timer--;

					//entity->cShape->Circle.setFillColor(sf::Color(fillColor.r, fillColor.g, fillColor.b, fillAlpha));
					//entity->cShape->Circle.setOutlineColor(sf::Color(outlineColor.r, outlineColor.g, outlineColor.b, outlineAlpha));

					break;
				}

				default:
					break;
			}

			totalTime--;
		}
	}
}