#pragma once

#include <map>
#include <vector>
#include <memory>

#include <fstream>
#include <json.hpp>
using json = nlohmann::json;

#include <SFML/Graphics.hpp>
#include "Core/Math.h"

namespace Eero {

	struct AnimationData
	{
		std::string TexName = "default";
		Vec2 TexRectPos = { 0.0f, 0.0f };
		Vec2 TexRectSize = { 0.0f, 0.0f };
		Vec2 States = { 0, 0 };
		float Speed = 0.0f;
	};

	using TextureMap = std::map<std::string, std::shared_ptr<sf::Texture>>;
	using FontMap = std::map<std::string, std::shared_ptr<sf::Font>>;
	using AnimationMap = std::map<std::string, AnimationData>;

	class Assets
	{
	public:
		Assets();
		~Assets();

		static std::shared_ptr<sf::Texture>& GetTexture(const std::string& textureName) { return s_Textures[textureName]; }
		static std::shared_ptr<sf::Font>& GetFont(const std::string& fontName) { return s_Fonts[fontName]; }
		static AnimationData& GetAnimation(const std::string& animationName) { return s_Animations[animationName]; }
	private:
		bool CheckFileFormatting(const json& data);

		void LoadTextures(const json& data);
		void LoadFonts(const json& data);
		void LoadAnimations(const json& data);
	private:
		TextureMap m_Textures;
		FontMap m_Fonts;
		AnimationMap m_Animations;
		
		static TextureMap s_Textures;
		static FontMap s_Fonts;
		static AnimationMap s_Animations;

	};

}
