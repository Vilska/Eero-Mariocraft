#pragma once

#include <SFML/Graphics.hpp>

#include "Core/Math.h"

namespace Eero {

	class Window
	{
	public:
		Window(const std::string& title, float width, float height);
		~Window();

		void Shutdown();
		void Clear();
		void Display();

		template<typename... Args>
		void Draw(Args&&... args) { m_Window->draw(std::forward<Args>(args)...); }

		bool PollEvent(sf::Event& eventX) { return m_Window->pollEvent(eventX); }

		void SetBackgroundColor(const Vec3& bgColor) { m_BackgroundColor = { (sf::Uint8)bgColor.x, (sf::Uint8)bgColor.y, (sf::Uint8)bgColor.z }; }

		void SetSize(float width, float height);
		const Vec2& GetSize() const { return Vec2(m_Width, m_Height); }
	private:
		void Init(const std::string& title, float width, float height);
	private:
		std::shared_ptr<sf::RenderWindow> m_Window;
		float m_Width, m_Height = 0.0f;
		sf::Color m_BackgroundColor = { 0, 0, 0 };
	};

}
