#include "Components.h"
#include "Entity.h"
#include "Core/Time.h"
#include "Core/Log.h"

namespace Eero {

	BaseComponent::BaseComponent(Entity& entity)
		: m_EntityInstance(entity) {}

	TransformComponent::TransformComponent(Entity& entity, const Vec2& pos, const Vec2& velocity, const Vec2& gravity, float maxSpeed, float angle)
		: BaseComponent(entity), Pos(pos), Velocity(velocity), Gravity(gravity), MaxSpeed(maxSpeed), Angle(angle) {}

	SpriteComponent::SpriteComponent(Entity& entity, const std::string& textureName, int direction)
		: BaseComponent(entity), TextureName(textureName), Direction(direction)
	{
		SetSprite(true);
	}

	SpriteComponent::SpriteComponent(Entity& entity, const std::string& textureName, const Vec2& pos, const Vec2& size, int direction)
		: BaseComponent(entity), TextureName(textureName), TexRectPos(pos), TexRectSize(size), Direction(direction)
	{
		SetSprite(false);
	}

	void SpriteComponent::SetSprite(bool singular)
	{
		Sprite = std::make_shared<sf::Sprite>();
		std::shared_ptr<sf::Texture> texture = Assets::GetTexture(TextureName);

		if (texture == nullptr)
		{
			LOG_ERROR("(Sprite) - Texture '{0}' does not exists!", TextureName);
			return;
		}

		Sprite->setTexture(*texture);

		if (singular)
		{
			Vec2 actualPos = { 0.0f, 0.0f };

			if (Direction == -1)
			{
				actualPos.x = texture->getSize().x;
			}

			Sprite->setTextureRect(sf::IntRect(actualPos.x, actualPos.y, Direction * texture->getSize().x, texture->getSize().y));
		}
		else
		{
			if (Direction == -1)
			{
				TexRectPos.x += TexRectSize.x;
				TexRectSize.x *= Direction;

				Sprite->setTextureRect(sf::IntRect(TexRectPos.x, TexRectPos.y, TexRectSize.x, TexRectSize.y));

				// Set to default
				TexRectSize.x *= Direction;
				TexRectPos.x -= TexRectSize.x;
			}
			else
			{
				Sprite->setTextureRect(sf::IntRect(TexRectPos.x, TexRectPos.y, TexRectSize.x, TexRectSize.y));
			}
		}
	}

	AnimationComponent::AnimationComponent(Entity& entity, const std::string& animationName, int direction)
		: BaseComponent(entity)
	{
		// Get the data from assets and assign proper variables
		auto data = Assets::GetAnimation(animationName);
		auto spriteComponent = m_EntityInstance.GetComponent<SpriteComponent>();

		// Error checking
		if (data.TexName == "default")
		{
			LOG_ERROR("(Animation) - Animation {0} does not exists!", animationName);
			return;
		}

		if (spriteComponent == nullptr)
		{
			LOG_ERROR("(Animation) - Entity does not contain the SpriteComponent!");
			return;
		}

		Sprite = spriteComponent->Sprite;
		TexName = data.TexName;
		TexRectPos = Convert::GridToPixels(data.TexRectPos, Vec2(Sprite->getTexture()->getSize().x, Sprite->getTexture()->getSize().y), data.TexRectSize.x);
		TexRectSize = data.TexRectSize;
		States = data.States;
		Speed = Time::Seconds(data.Speed);
		Direction = direction;

		// Save previous texture data
		OrgTextureName = spriteComponent->TextureName;
		OrgTexRect = {(int)spriteComponent->TexRectPos.x, (int)spriteComponent->TexRectPos.y, (int)spriteComponent->TexRectSize.x, (int)spriteComponent->TexRectSize.y};

		// Texture change
		auto texture = Assets::GetTexture(TexName);

		// Check if texture exists
		if (texture == nullptr)
		{
			LOG_ERROR("(Animation) - Texture '{0}' does not exist!", TexName);
			return;
		}

		// If the texture differs, change it
		if (Sprite->getTexture() != texture.get())
		{
			Sprite->setTexture(*texture);
			TexRectPos = Convert::GridToPixels(data.TexRectPos, Vec2(texture->getSize().x, texture->getSize().y), data.TexRectSize.x);
			TextureChanged = true;
		}
	}

	AnimationComponent::~AnimationComponent()
	{
		auto texture = Assets::GetTexture(OrgTextureName);

		// Set original texture
		if (TextureChanged)
		{
			Sprite->setTexture(*texture);
		}

		// Calculate texture offset/direction
		if (Direction == -1)
		{
			if (TexRectSize.x == 0) // Singular?
				OrgTexRect.left += texture->getSize().x;
			else
				OrgTexRect.left += TexRectSize.x;
		}

		OrgTexRect.width *= Direction;

		// Set original TextureRect
		Sprite->setTextureRect(OrgTexRect);
	}

	CollisionComponent::CollisionComponent(Entity& entity, const Vec2& size)
		: BaseComponent(entity), Size(size), HalfSize({ Size.x / 2, Size.y / 2 }) {}

	LifespanComponent::LifespanComponent(Entity& entity, int total, int action, LifespanComponent::EffectTypes effect)
		: BaseComponent(entity), TotalTime(total), ActionTime(action), Effect(effect) {}

	TextComponent::TextComponent(Entity& entity, const std::string& fontName, const std::string& text, const Vec2& position, const Vec3& color, int size, bool centred)
		: BaseComponent(entity), FontName(fontName), TextStr(text), Position(position), Color(color), Size(size), Centred(centred)
	{
		std::shared_ptr<sf::Font> font = Assets::GetFont(FontName);

		if (font == nullptr)
		{
			LOG_ERROR("(Text) - Font '{0}' does not exist!", FontName);
		}

		Text.setFont(*font);
		Text.setString(TextStr);
		Text.setFillColor(sf::Color(Color.x, Color.y, Color.z));
		Text.setCharacterSize(Size);

		if (centred)
		{
			auto textRect = Text.getGlobalBounds();
			Text.setOrigin(round(textRect.left + textRect.width / 2.0f), round(textRect.top + textRect.height / 2.0f));
		}

		Text.setPosition(sf::Vector2(Position.x, Position.y));
	}

	void TextComponent::SetText(const std::string& text)
	{
		Text.setString(text);
	}
	
	void TextComponent::SetColor(const Vec3& color)
	{
		Text.setFillColor(sf::Color(color.x, color.y, color.z));
	}

}