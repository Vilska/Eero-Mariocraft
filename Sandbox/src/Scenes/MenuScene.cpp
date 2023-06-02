#include "MenuScene.h"

namespace Eero {

	void MenuScene::Load()
	{
		Application::GetWindow()->SetBackgroundColor(Vec3(0, 0, 0));
		CreateLogo();
		CreateText();
	}

	void MenuScene::Update()
	{
		UserInput();
	}

	void MenuScene::Unload()
	{
		for (auto& e : Application::GetEntityManager()->GetEntities())
		{
			e->Destroy(); // Destroy all the entities when changing scene
		}

		m_Texts.clear();
		m_TextIndex = 0;
	}

	void MenuScene::CreateLogo()
	{
		auto menuLogo = Application::GetEntityManager()->PushEntity("menuLogo");
		menuLogo->AddComponent<SpriteComponent>("logo", Vec2(0, 0), Vec2(561, 462));

		const Vec2 windowSize = Application::GetWindow()->GetSize();
		menuLogo->AddComponent<TransformComponent>(Vec2(windowSize.x / 3, windowSize.y / 5), Vec2(0.0f, 0.0f), Vec2(0.0f, 0.0f), 0, 0);
	}

	void MenuScene::CreateText()
	{
		const Vec2 windowSize = Application::GetWindow()->GetSize();

		// PlayText
		auto playText = Application::GetEntityManager()->PushEntity("playText");
		playText->AddComponent<TextComponent>("Pixeloid", "Play game", Vec2((windowSize.x / 2) - 40, windowSize.y - windowSize.y / 4), Vec3(255, 255, 255), 60, true);

		// InfoText
		auto infoText = Application::GetEntityManager()->PushEntity("infoText");
		infoText->AddComponent<TextComponent>("Pixeloid", "About", Vec2((windowSize.x / 2) - 40, windowSize.y - windowSize.y / 6), Vec3(255, 255, 255), 60, true);

		m_Texts.push_back(playText);
		m_Texts.push_back(infoText);
		CheckTextIndex();
	}

	void MenuScene::CreateAbout()
	{
		Unload();
		m_AboutLoaded = true;

		auto aboutBox = Application::GetEntityManager()->PushEntity("aboutBox");
		aboutBox->AddComponent<SpriteComponent>("about");

		const Vec2 windowSize = Application::GetWindow()->GetSize();
		aboutBox->AddComponent<TransformComponent>(Vec2(windowSize.x / 3, windowSize.y / 5), Vec2(0.0f, 0.0f), Vec2(0.0f, 0.0f), 0, 0);
	}

	void MenuScene::CheckTextIndex()
	{
		for (int i = 0; i < m_Texts.size(); i++)
		{
			if (i == m_TextIndex)
				m_Texts[i]->GetComponent<TextComponent>()->SetColor(Vec3(229, 133, 21));
			else
				m_Texts[i]->GetComponent<TextComponent>()->SetColor(m_Texts[i]->GetComponent<TextComponent>()->Color);
		}
	}

	void MenuScene::UserInput()
	{
		auto& input = Application::GetInput();

		if (input->KeyPressed(KEY_Escape) && m_AboutLoaded)
		{
			Unload();
			m_AboutLoaded = false;

			Load();

			m_TextIndex = 1;
			CheckTextIndex();
		}

		if (m_AboutLoaded)
			return;

		if (input->KeyPressed(KEY_Up))
		{
			if (m_TextIndex == 0)
				return;

			m_TextIndex--;
			CheckTextIndex();
		}

		if (input->KeyPressed(KEY_Down))
		{
			if (m_TextIndex == 1)
				return;

			m_TextIndex++;
			CheckTextIndex();
		}

		if (input->KeyPressed(KEY_Enter))
		{
			EnterAction(m_TextIndex);
		}

	}

	void MenuScene::EnterAction(int index)
	{
		switch (index)
		{
			case 0: // Play game
			{
				ChangeSceneData(1); // PlayScene

				break;
			}

			case 1:
			{
				CreateAbout();
			}

			default:
				break;
		}
	}

}