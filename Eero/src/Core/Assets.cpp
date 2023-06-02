#include "Assets.h"
#include "Core/Log.h"

#include <exception>

namespace Eero {

	TextureMap Assets::s_Textures = {};
	FontMap Assets::s_Fonts = {};
	AnimationMap Assets::s_Animations = {};

	Assets::Assets()
	{
		std::ifstream file("assets/assets.json");

		// Error checks
		if (!file.is_open())
		{
			LOG_ERROR("(Assets) - 'assets.json' could not be found!");
			return;
		}

		json data;
		try
		{
			data = json::parse(file);
		}
		catch (json::parse_error)
		{
			LOG_ERROR("(Assets) - 'assets.json' badly formatted!");
			return;
		}

		if (CheckFileFormatting(data))
			return;

		// Load
		LoadTextures(data["textures"]);
		LoadFonts(data["fonts"]);
		LoadAnimations(data["animations"]);

		s_Textures = m_Textures;
		s_Fonts = m_Fonts;
		s_Animations = m_Animations;
	}

	Assets::~Assets()
	{
		s_Textures = {};
		s_Fonts = {};
	}

	bool Assets::CheckFileFormatting(const json& data)
	{
		if (!data.size() > 0)
		{
			LOG_ERROR("(Assets) - 'assets.json' badly formatted!");
			return true;
		}

		try
		{
			data.at("textures");
		}
		catch (const std::exception& e)
		{
			LOG_ERROR("(Assets) - 'Textures' section could not be found in the assets file!");
			return true;
		}

		try
		{
			data.at("fonts");
		}
		catch (const std::exception& e)
		{
			LOG_ERROR("(Assets) - 'Fonts' section could not be found in the assets file!");
			return true;
		}

		try
		{
			data.at("animations");
		}
		catch (const std::exception& e)
		{
			LOG_ERROR("(Assets) - 'Animations' section could not be found in the assets file!");
			return true;
		}

		return false;
	}

	void Assets::LoadTextures(const json& data)
	{
		if (!data.size() > 0)
		{
			LOG_ERROR("(Assets) - 'Textures' does not contain any data!");
			return;
		}

		for (int i = 0; i < data.size(); i++)
		{
			try
			{
				data[i].at(0);
				data[i].at(1);
			}
			catch (const std::exception& e)
			{
				LOG_ERROR("(Assets) - 'Textures' component '{0}' badly formatted!", i);
				return;
			}

			auto texture = std::make_shared<sf::Texture>();

			std::string textureFileName = data[i][1];

			if (!texture->loadFromFile("assets/textures/" + textureFileName))
				LOG_ERROR("(Assets) - Texture named '{0}' could not be found!", textureFileName);

			m_Textures[data[i][0]] = texture;
		}
	}

	void Assets::LoadFonts(const json& data)
	{
		if (!data.size() > 0)
		{
			LOG_ERROR("(Assets) - 'Fonts' does not contain any data!");
			return;
		}

		for (int i = 0; i < data.size(); i++)
		{
			try
			{
				data[i].at(0);
				data[i].at(1);
			}
			catch (const std::exception& e)
			{
				LOG_ERROR("(Assets) - 'Fonts' component '{0}' badly formatted!", i);
				return;
			}

			auto font = std::make_shared<sf::Font>();

			std::string fontFileName = data[i][1];

			if (!font->loadFromFile("assets/fonts/" + fontFileName))
				LOG_ERROR("(Assets) - Font named '{0}' could not be found!", fontFileName);

			m_Fonts[data[i][0]] = font;
		}
	}

	void Assets::LoadAnimations(const json& data)
	{
		if (!data.size() > 0)
		{
			LOG_ERROR("(Assets) - 'Animations' does not contain any data!");
			return;
		}

		for (int i = 0; i < data.size(); i++)
		{
			try
			{
				data[i].at(0);
				data[i].at(1);
				data[i].at(2);
				data[i].at(3);
				data[i].at(4);
				data[i].at(5);
			}
			catch (const std::exception& e)
			{
				LOG_ERROR("(Assets) - 'Animations' component '{0}' badly formatted!", i);
				return;
			}

			AnimationData animation
			(
				data[i][1],
				Vec2(data[i][2][0], data[i][2][1]),
				Vec2(data[i][3][0], data[i][3][1]),
				Vec2(data[i][4][0], data[i][4][1]),
				data[i][5]
			);

			m_Animations[data[i][0]] = animation;
		}
	}

}
