#pragma once

#include <SFML/Graphics.hpp>

#include "Core/Assets.h"

namespace Eero {

	class Entity;

	struct BaseComponent
	{
		BaseComponent() = default;
		BaseComponent(Entity& entity);

		virtual ~BaseComponent() {}

	protected:
		Entity& m_EntityInstance;
	};

	struct TransformComponent : public BaseComponent
	{
		Vec2 Pos = { 0.0f, 0.0f };
		Vec2 PrevPos = { 0.0f, 0.0f };
		Vec2 Velocity = { 0.0f, 0.0f };
		Vec2 Gravity = { 0.0f, 0.0f };
		float MaxSpeed, Angle = 0.0f;

		TransformComponent() = default;
		TransformComponent(Entity& entity, const Vec2& pos, const Vec2& velocity, const Vec2& gravity, float maxSpeed, float angle);
	};

	struct SpriteComponent : public BaseComponent
	{
		std::string TextureName = "default";
		Vec2 TexRectPos = { 0.0f, 0.0f };
		Vec2 TexRectSize = { 0.0f, 0.0f };
		int Direction = 1;

		std::shared_ptr<sf::Sprite> Sprite;

		SpriteComponent() = default;
		SpriteComponent(Entity& entity, const std::string& textureName, int direction = 1);
		SpriteComponent(Entity& entity, const std::string& textureName, const Vec2& pos, const Vec2& size, int direction = 1);
	private:
		void SetSprite(bool singular);
	};

	struct AnimationComponent : public BaseComponent
	{
		// Animation specific data
		std::shared_ptr<sf::Sprite> Sprite;
		std::string TexName = "default";
		Vec2 TexRectPos = { 0.0f, 0.0f };
		Vec2 TexRectSize = { 0.0f, 0.0f };
		Vec2 States = { 0, 0 };
		int Speed, FrameCount = 0; 
		int sXIndex = 0;
		int sYIndex = 0;
		int Direction = 1;
		bool TextureChanged = false;

		std::string OrgTextureName;
		sf::IntRect OrgTexRect;

		AnimationComponent() = default;
		AnimationComponent(Entity& entity, const std::string& animationName, int direction = 1);
		~AnimationComponent();
	};

	struct CollisionComponent : public BaseComponent // AABB
	{
		Vec2 Size = { 0.0f, 0.0f };
		Vec2 HalfSize = { 0.0f, 0.0f };
		bool Handled = false;

		CollisionComponent() = default;
		CollisionComponent(Entity& entity, const Vec2& size);
	};

	struct LifespanComponent : public BaseComponent
	{
		int TotalTime, ActionTime = 0;

		enum class EffectTypes
		{
			Disappear = 0, Fade = 1, Blink = 2
		};

		EffectTypes Effect = EffectTypes::Disappear;

		LifespanComponent() = default;
		LifespanComponent(Entity& entity, int total, int action, LifespanComponent::EffectTypes effect);

	};

	struct TextComponent : public BaseComponent
	{
		sf::Text Text;

		std::string FontName;
		std::string TextStr;
		Vec2 Position;
		Vec3 Color;
		int Size = 0;
		bool Centred = false;

		TextComponent() = default;
		TextComponent(Entity& entity, const std::string& fontName, const std::string& text, const Vec2& position, const Vec3& color, int size, bool centred);

		void SetText(const std::string& text);
		void SetColor(const Vec3& color);
	};

}
