#include "Collision.h"

namespace Eero {

	void Collision::CheckCollision(const std::string& tagX, const std::string& tagY, bool continuously, const std::function<void(const CollisionData)>& func)
	{
		for (auto& entityX : m_EntityManager->GetEntities(tagX))
		{
			for (auto& entityY : m_EntityManager->GetEntities(tagY))
			{
				if (!entityX->HasComponent<CollisionComponent>() || !entityY->HasComponent<CollisionComponent>())
					continue;

				auto entityXCollision = entityX->GetComponent<CollisionComponent>();
				auto entityYCollision = entityY->GetComponent<CollisionComponent>();
				auto entityXTransform = entityX->GetComponent<TransformComponent>();
				auto entityYTransform = entityY->GetComponent<TransformComponent>();

				Vec2 overlap = CalculateOverlap({ entityXTransform->Pos, entityYTransform->Pos }, { entityXCollision->HalfSize, entityYCollision->HalfSize });

				if (!(overlap.x > 0 && overlap.y > 0))
				{
					if (entityXCollision->Handled && entityYCollision->Handled) // if two entities have had collision
					{
						entityXCollision->Handled = false;
						entityYCollision->Handled = false;
					}

					continue;
				}

				if (!continuously)
				{
					if (entityXCollision->Handled && entityYCollision->Handled)
					{
						continue;
					}

					entityXCollision->Handled = true;
					entityYCollision->Handled = true;
				}

				CollisionData collision(entityX, entityY, overlap);
				CalculateDirection(collision);

				func(collision);
			}
		}
	}

	const Vec2& Collision::CalculateOverlap(const std::tuple<Vec2, Vec2>& positions, const std::tuple<Vec2, Vec2>& sizes)
	{
		auto& [pos1, pos2] = positions;
		auto& [size1, size2] = sizes;

		Vec2 delta =
		{
			abs((pos1.x + size1.x) - (pos2.x + size2.x)),
			abs((pos1.y + size1.y) - (pos2.y + size2.y))
		};

		float overlapX = (size1.x + size2.x) - delta.x;
		float overlapY = (size1.y + size2.y) - delta.y;

		return Vec2(overlapX, overlapY);
	}

	void Collision::CalculateDirection(CollisionData& collisionData)
	{
		auto& prevFramePosX = collisionData.EntityX->GetComponent<TransformComponent>()->PrevPos;
		auto& prevFramePosY = collisionData.EntityY->GetComponent<TransformComponent>()->PrevPos;
		auto& collisionHalfSizeX = collisionData.EntityX->GetComponent<CollisionComponent>()->HalfSize;
		auto& collisionHalfSizeY = collisionData.EntityY->GetComponent<CollisionComponent>()->HalfSize;

		// Pre frame collision
		// We suppose that the check will be made within the next frame from detected collision
		Vec2 prevFrameOverlap = CalculateOverlap({ prevFramePosX, prevFramePosY }, { collisionHalfSizeX, collisionHalfSizeY });

		if (prevFrameOverlap.x > 0 && (prevFramePosX.y < prevFramePosY.y)) // TOP
		{
			collisionData.Direction = CollisionData::Direction::Top;
		}
		else if (prevFrameOverlap.x > 0 && (prevFramePosX.y > prevFramePosY.y)) // BOTTOM
		{
			collisionData.Direction = CollisionData::Direction::Bottom;
		}
		else if (prevFrameOverlap.y > 0 && (prevFramePosX.x < prevFramePosY.x)) // LEFT
		{
			collisionData.Direction = CollisionData::Direction::Left;
		}
		else if (prevFrameOverlap.y > 0 && (prevFramePosX.x > prevFramePosY.x)) // RIGHT
		{
			collisionData.Direction = CollisionData::Direction::Right;
		}
	}

}