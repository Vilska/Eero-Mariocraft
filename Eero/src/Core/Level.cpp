#include "Level.h"
#include "Log.h"
#include "Assets.h"

#include <exception>

#include <fstream>
#include <json.hpp>
using json = nlohmann::json;

namespace Eero {

	LevelSprites Level::Create(const std::string& levelFilename)
	{
		LevelSprites levelSprites;

		// Open the file
		std::ifstream file("levels/" + levelFilename);

		// Error checks
		if (!file.is_open())
		{
			LOG_ERROR("(Level) - " + levelFilename + " could not be found!");
			return levelSprites;
		}

		json data;
		try
		{
			data = json::parse(file);
		}
		catch (json::parse_error)
		{
			LOG_ERROR("(Level) - " + levelFilename + " badly formatted!");
			return levelSprites;
		}

		try
		{
			data.at("sprites");
		}
		catch (const std::exception& e)
		{
			LOG_ERROR("(Level) - 'Sprites' could not be found!");
			return levelSprites;
		}

		if (!data["sprites"].size() > 0)
		{
			LOG_ERROR("(Level) - 'Sprites' does not contain any data!");
			return levelSprites;
		}

		// Parse data
		for (int i = 0; i < data["sprites"].size(); i++)
		{
			auto& sprite = data["sprites"][i];

			try
			{
				sprite.at("name");
				sprite.at("position");
				sprite.at("texName");
				sprite.at("texRectCoords");
				sprite.at("texRectSize");
			}
			catch (const std::exception& e)
			{
				LOG_ERROR("(Level) - 'Sprites' section badly formatted!");
				return levelSprites;
			}

			auto& texture = Assets::GetTexture(sprite["texName"]);
			if (texture == nullptr)
			{
				LOG_ERROR("(Sprite) - Texture '{0}' does not exist!", sprite["texName"]);
				return levelSprites;
			}

			auto size = texture->getSize();

			LevelSpriteData levelSprite(
				sprite["name"],
				Vec2(sprite["position"][0], sprite["position"][1]),
				sprite["texName"],
				Vec2(sprite["texRectCoords"][0], sprite["texRectCoords"][1]),
				Vec2(sprite["texRectSize"][0], sprite["texRectSize"][1]),
				Vec2(size.x, size.y)
			);

			levelSprites.push_back(levelSprite);

		}

		return levelSprites;
	}

}