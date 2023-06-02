#pragma once

#include "Core/Application.h"

namespace Eero {

	struct Player
	{
		enum class State
		{
			None, Idle, Running, Air, Fall
		};

		enum class Direction
		{
			None, Left, Right
		};
	};

	using PlayerState = Player::State;
	using PlayerDirection = Player::Direction;

	class PlayScene : public Scene
	{
	public:
		virtual void Load() override;
		virtual void Update() override;
		virtual void Unload() override;
	private:
		void CreatePlayer();
		void CreateLevel();
		void CreateBullet();

		void Collisions();
		void UserInput();

		int SetDirection(PlayerDirection direction);
	private:
		std::shared_ptr<Level> m_Level;

		std::shared_ptr<Entity> m_Player;
		Player::State m_PlayerState = Player::State::None;
		Player::Direction m_PlayerDirection = Player::Direction::None; // Stores player "heading" so texture can be flipped correctly
	};

}