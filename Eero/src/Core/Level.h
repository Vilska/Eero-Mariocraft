#pragma once

#include <string>
#include <vector>

#include "Math.h"

namespace Eero {

	struct LevelSpriteData
	{
		std::string Name = "default";
		Vec2 Position = {0.0f, 0.0f};
		std::string TexName = "default";
		Vec2 TexRectCoords = { 0.0f, 0.0f };
		Vec2 TexRectSize = { 0.0f, 0.0f };
		Vec2 TextureSize = { 0.0f, 0.0f };

		LevelSpriteData(const std::string& name, const Vec2& position, const std::string& texName, const Vec2& texRectCoords, const Vec2& texRectSize, const Vec2& textureSize)
			: Name(name), Position(position), TexName(texName), TexRectCoords(texRectCoords), TexRectSize(texRectSize), TextureSize(textureSize) {}
	};

	typedef std::vector<LevelSpriteData> LevelSprites;

	class Level
	{
	public:
		static LevelSprites Create(const std::string& levelFilename);
	};

}
