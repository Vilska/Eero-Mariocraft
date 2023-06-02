#include "Game.h"

namespace Eero {

	void Game::OnAttach()
	{
		m_MenuScene = std::make_shared<MenuScene>();
		m_PlayScene = std::make_shared<PlayScene>();

		Scene::ChangeScene(m_MenuScene);
	}

	void Game::OnUpdate(float deltaTime)
	{
		int changeSceneTo = Scene::CheckStatus();

		if (changeSceneTo == -1)
			return;

		switch (changeSceneTo)
		{
			case 0: // MenuScene
			{
				Scene::ChangeScene(m_MenuScene);

				break;
			}

			case 1: // PlayScene
			{
				Scene::ChangeScene(m_PlayScene);

				break;
			}

			default:
				break;
		}
	}

	std::shared_ptr<Application> CreateApplication()
	{
		AppProps props = {"Mariocraft", 1920.0f, 1080.0f};
		std::shared_ptr<Application> app = std::make_shared<Application>(props);

		app->PushLayer<Game>();

		return app;
	}


}
