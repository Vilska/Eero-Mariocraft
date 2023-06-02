#pragma once

#include "ECS/EntityManager.h"

namespace Eero {

	struct CollisionData
	{
		std::shared_ptr<Entity> EntityX;
		std::shared_ptr<Entity> EntityY;
		Vec2 Overlap;

		enum class Direction
		{
			None, Top, Bottom, Left, Right
		};

		CollisionData::Direction Direction;
	};

	class Collision
	{
		friend class Systems;
	public:
		void CheckCollision(const std::string& tagX, const std::string& tagY, bool continuously, const std::function<void(const CollisionData)>& func);
	private:
		Collision(const std::shared_ptr<EntityManager>& entityManager)
			: m_EntityManager(entityManager) {}

		const Vec2& CalculateOverlap(const std::tuple<Vec2, Vec2>& positions, const std::tuple<Vec2, Vec2>& sizes);
		void CalculateDirection(CollisionData& collisionData);
	private:
		std::vector<std::shared_ptr<CollisionData>> m_Collisions;
		std::shared_ptr<EntityManager> m_EntityManager;
	};

}
