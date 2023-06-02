#pragma once

#include "Time.h"
#include "Layer.h"
#include "Assets.h"
#include "Scene.h"

#include "Window/Window.h"

#include "ECS/EntityManager.h"
#include "ECS/Systems/Systems.h"

#include "Event/EventHandler.h"
#include "Event/Input.h"

#include "Event/KeyMouseCodes.h"

namespace Eero {

	struct AppProps
	{
		std::string WindowTitle = "Test";
		float WindowWidth = 1280.0f;
		float WindowHeight = 720.0f;
	};

	class Application
	{
	public:
		Application(const AppProps& props);
		~Application();

		void Run();

		template<typename T>
		void PushLayer()
		{
			static_assert(std::is_base_of<Layer, T>::value, "Pushed type is not subclass of Layer!");
			m_Layers.emplace_back(std::make_shared<T>())->OnAttach();
		}

		static Application* GetInstance() { return s_Instance; }

		static std::shared_ptr<EntityManager>& GetEntityManager() { return s_Instance->m_EntityManager; }
		static std::shared_ptr<Input>& GetInput() { return s_Instance->m_Input; }
		static std::shared_ptr<Window>& GetWindow() { return s_Instance->m_Window; }
		static std::shared_ptr<Collision>& GetCollision() { return s_Instance->m_Collision; }
	private:
		void Init(const AppProps& props);
		void Shutdown();

		void CheckWindowEvents();
	private:
		std::shared_ptr<Window> m_Window;
		std::shared_ptr<EventHandler> m_Events;
		std::shared_ptr<EntityManager> m_EntityManager;
		std::shared_ptr<Input> m_Input;
		std::shared_ptr<Systems> m_Systems;
		std::shared_ptr<Assets> m_Assets;
		std::shared_ptr<Collision> m_Collision;
		std::shared_ptr<Scene> m_Scene;

		std::vector<std::shared_ptr<Layer>> m_Layers;

		bool m_Running = true;
		float m_Timestep = 0.0f;

		// Instance
		static Application* s_Instance;
	};

	std::shared_ptr<Application> CreateApplication();

}
