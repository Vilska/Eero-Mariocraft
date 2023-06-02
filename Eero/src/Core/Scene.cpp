#include "Scene.h"

#include <iostream>

namespace Eero {

	Scene* Scene::s_Instance = nullptr;

	Scene::Scene()
	{
		s_Instance = this;
	}

	void Scene::ChangeScene(const std::shared_ptr<Scene>& newScene)
	{
		if (s_Instance->m_CurrentScene != NULL)
			s_Instance->m_CurrentScene->Unload();

		s_Instance->m_CurrentScene = newScene;
		s_Instance->m_CurrentScene->Load();

		s_Instance->m_Status = Scene::Status::Standby;
	}

	int Scene::CheckStatus()
	{
		if (s_Instance->m_Status == Scene::Status::Change)
			return s_Instance->m_ChangeSceneTo;

		return -1;
	}

	void Scene::Update()
	{
		s_Instance->m_CurrentScene->Update();
	}

}