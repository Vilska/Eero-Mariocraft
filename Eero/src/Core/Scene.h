#pragma once

#include <string>
#include <memory>

#include "Level.h"

namespace Eero {

	class Scene
	{
	public:
		Scene();

		static void ChangeScene(const std::shared_ptr<Scene>& newScene);
		static int CheckStatus();

		virtual void Update();

		virtual void Load() {}
		virtual void Unload() {}
	protected:
		enum class Status
		{
			None = 0, Standby = 1, Change = 2
		};

		Scene::Status m_Status = Scene::Status::None;

		void ChangeSceneData(int changeTo)
		{
			s_Instance->m_Status = Status::Change;
			s_Instance->m_ChangeSceneTo = changeTo;
		}

		int m_ChangeSceneTo = 0;
	private:
		static Scene* s_Instance;
		std::shared_ptr<Scene> m_CurrentScene;
	};

}
