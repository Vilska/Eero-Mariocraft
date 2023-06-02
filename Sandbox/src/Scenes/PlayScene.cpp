#include "PlayScene.h"
#include "Core/Log.h"

namespace Eero {

	void PlayScene::Load()
	{
		CreateLevel();
		CreatePlayer();
	}

	void PlayScene::CreatePlayer()
	{
		auto entity = Application::GetEntityManager()->PushEntity("player");
		entity->AddComponent<SpriteComponent>("mario", Vec2(0.0f, 0.0f), Vec2(128.0f, 128.0f));
		entity->AddComponent<TransformComponent>(Convert::GridToPixels(Vec2(0, 7), Application::GetWindow()->GetSize(), 128), Vec2(0.0f, 0.0f), Vec2(0.0f, 800.0f), 300, 0);
		entity->AddComponent<CollisionComponent>(Vec2(128.0f, 128.0f));

		m_Player = entity;
		m_PlayerState = PlayerState::Idle;
		m_PlayerDirection = PlayerDirection::Left;
	}

	void PlayScene::CreateLevel()
	{
		Application::GetWindow()->SetBackgroundColor(Vec3(51, 153, 255));

		// Level blocks
		auto levelSprites = Level::Create("level1.json");

		for (auto& sprite : levelSprites)
		{
			auto actualPos = Convert::GridToPixels(sprite.Position, Application::GetWindow()->GetSize(), 128);
			auto actualTexRectCoords = Convert::GridToPixels(sprite.TexRectCoords, sprite.TextureSize, 128);

			auto entity = Application::GetEntityManager()->PushEntity(sprite.Name);
			entity->AddComponent<SpriteComponent>(sprite.TexName, actualTexRectCoords, sprite.TexRectSize);
			entity->AddComponent<TransformComponent>(actualPos, Vec2(0.0f, 0.0f), Vec2(0.0f, 0.0f), 0, 0);
			entity->AddComponent<CollisionComponent>(sprite.TexRectSize);
		}
	}

	void PlayScene::CreateBullet()
	{
		auto entity = Application::GetEntityManager()->PushEntity("bullet");
		entity->AddComponent<SpriteComponent>("bullet", SetDirection(m_PlayerDirection));

		auto& playerPos = m_Player->GetComponent<TransformComponent>()->Pos;
		auto [mouseX, mouseY] = Application::GetInput()->GetMousePosition();
		Vec2 mousePos = { mouseX, mouseY };

		Vec2 difference = mousePos - playerPos;
		Vec2 normal = { difference.x / difference.length(), difference.y / difference.length() };
		Vec2 velocity = { 600.0f * normal.x, 0 };
		
		entity->AddComponent<TransformComponent>(Vec2(playerPos.x, playerPos.y + (m_Player->GetComponent<SpriteComponent>()->TexRectSize.y / 2)), velocity, Vec2(0.0f, 0.0f), 1000, 0);

		entity->AddComponent<LifespanComponent>(Time::Seconds(2), Time::Seconds(2), LifespanComponent::EffectTypes::Disappear);
		entity->AddComponent<CollisionComponent>(Vec2(32.0f, 16.0f));
	}

	void PlayScene::Update()
	{
		Collisions();
		UserInput();
	}

	void PlayScene::Unload()
	{
		for (auto& e : Application::GetEntityManager()->GetEntities())
		{
			e->Destroy(); // Destroy all the entities when changing scene
		}
	}

	void PlayScene::Collisions()
	{
		Application::GetCollision()->CheckCollision("player", "block", true, [&](const CollisionData& collision)
		{
			auto& player = collision.EntityX;
			auto& block = collision.EntityY;
			auto& overlap = collision.Overlap;

			switch (collision.Direction)
			{
				case CollisionData::Direction::Top:
				{
					player->GetComponent<TransformComponent>()->Pos.y -= overlap.y;

					if (m_PlayerState == PlayerState::Air)
						m_PlayerState = PlayerState::Fall;

					break;
				}

				case CollisionData::Direction::Bottom:
				{
					player->GetComponent<TransformComponent>()->Pos.y += overlap.y;
					break;
				}

				case CollisionData::Direction::Left:
				{
					player->GetComponent<TransformComponent>()->Pos.x -= overlap.x;
					break;
				}

				case CollisionData::Direction::Right:
				{
					player->GetComponent<TransformComponent>()->Pos.x += overlap.x;
					break;
				}

				default:
					break;
			}

			if (m_PlayerState == PlayerState::Fall)
			{
				m_PlayerState = PlayerState::Idle;
				player->RemoveComponent<AnimationComponent>();
				player->GetComponent<TransformComponent>()->Velocity = { 0, 0 };
			}

		});

		Application::GetCollision()->CheckCollision("bullet", "block", false, [](const CollisionData& collision)
		{
			auto& bullet = collision.EntityX;
			auto& block = collision.EntityY;

			bullet->GetComponent<TransformComponent>()->Velocity = { 0.0f, 0.0f };
			bullet->AddComponent<LifespanComponent>(Time::Seconds(0.6), Time::Seconds(0.4), LifespanComponent::EffectTypes::Fade);

			block->AddComponent<AnimationComponent>("explosion");
			block->AddComponent<LifespanComponent>(Time::Seconds(0.9), Time::Seconds(0.9), LifespanComponent::EffectTypes::Disappear);
		});

		Application::GetCollision()->CheckCollision("player", "leaf", true, [&](const CollisionData& collision)
		{
			auto& player = collision.EntityX;
			auto& leaf = collision.EntityY;
			auto& overlap = collision.Overlap;

			switch (collision.Direction)
			{
				case CollisionData::Direction::Top:
				{
					player->GetComponent<TransformComponent>()->Pos.y -= overlap.y;

					if (m_PlayerState == PlayerState::Air)
						m_PlayerState = PlayerState::Fall;

					break;
				}

				case CollisionData::Direction::Bottom:
				{
					if (m_PlayerState != PlayerState::Air)
						return;

					player->GetComponent<TransformComponent>()->Velocity *= -1;
					leaf->AddComponent<LifespanComponent>(Time::Seconds(0.3), Time::Seconds(0.3), LifespanComponent::EffectTypes::Fade);

					m_PlayerState = PlayerState::Fall;

					break;
				}

				case CollisionData::Direction::Left:
				{
					player->GetComponent<TransformComponent>()->Pos.x -= overlap.x;
					break;
				}

				case CollisionData::Direction::Right:
				{
					player->GetComponent<TransformComponent>()->Pos.x += overlap.x;
					break;
				}

				default:
					break;
			}

			if (m_PlayerState == PlayerState::Fall)
			{
				m_PlayerState = PlayerState::Idle;
				player->RemoveComponent<AnimationComponent>();
				player->GetComponent<TransformComponent>()->Velocity = { 0, 0 };
			}

		});


		Application::GetCollision()->CheckCollision("player", "lava", false, [&](const CollisionData& collision)
		{
			ChangeSceneData(0);
		});
	}

	void PlayScene::UserInput()
	{
		auto& input = Application::GetInput();
		auto& velocity = m_Player->GetComponent<TransformComponent>()->Velocity;

		// A = move left
		if (input->KeyPressed(KEY_A))
		{
			if (m_PlayerState == PlayerState::Air)
				return;

			if (m_PlayerState != PlayerState::Running)
				m_Player->AddComponent<AnimationComponent>("marioRun", SetDirection(m_PlayerDirection = PlayerDirection::Left));

			m_PlayerState = PlayerState::Running;
			velocity.x = -300.0f;
		}

		// D = move right
		if (input->KeyPressed(KEY_D))
		{
			if (m_PlayerState == PlayerState::Air)
				return;

			if (m_PlayerState != PlayerState::Running)
				m_Player->AddComponent<AnimationComponent>("marioRun", SetDirection(m_PlayerDirection = PlayerDirection::Right));

			m_PlayerState = PlayerState::Running;
			velocity.x = 300.0f;
		}

		// SPACE = jump
		if (input->KeyPressed(KEY_Space) && m_PlayerState != PlayerState::Air)
		{
			m_PlayerState = PlayerState::Air;
			m_Player->AddComponent<AnimationComponent>("marioJump", SetDirection(m_PlayerDirection));

			velocity.y = -800.0f;
		}

		if (input->MouseButtonPressed(MOUSE_1))
		{
			CreateBullet();
		}

		if (input->KeyPressed(KEY_Escape))
		{
			ChangeSceneData(0); // Inform Scene that you want to change the scene to MenuScene
		}

		// Release A
		if (input->KeyReleased(KEY_A))
		{
			if (m_PlayerState == PlayerState::Air)
				return;

			m_PlayerState = PlayerState::Idle;
			m_Player->RemoveComponent<AnimationComponent>();

			velocity.x = 0;
		}

		// Release D
		if (input->KeyReleased(KEY_D))
		{
			if (m_PlayerState == PlayerState::Air)
				return;

			m_PlayerState = PlayerState::Idle;
			m_Player->RemoveComponent<AnimationComponent>();

			velocity.x = 0;
		}
	}

	int PlayScene::SetDirection(PlayerDirection direction)
	{
		switch (direction)
		{
			case PlayerDirection::Left:
				return -1;

			case PlayerDirection::Right:
				return 1;

			default:
				break;
		}
	}

}