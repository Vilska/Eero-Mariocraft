#pragma once

#include "Core/Application.h"

namespace Eero {

	class MenuScene : public Scene
	{
	public:
		virtual void Load() override;
		virtual void Update() override;
		virtual void Unload() override;
	private:
		void CreateLogo();
		void CreateText();
		void CreateAbout();

		void CheckTextIndex();

		void UserInput();

		void EnterAction(int index);
	private:
		int m_TextIndex = 0;
		std::vector<std::shared_ptr<Entity>> m_Texts;

		bool m_AboutLoaded = false;
	};

}
