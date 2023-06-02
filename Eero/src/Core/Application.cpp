#include "Application.h"

namespace Eero {

	 Application* Application::s_Instance = nullptr;

	Application::Application(const AppProps& props)
	{
		s_Instance = this;
		Init(props);
	}

	Application::~Application()
	{
		Shutdown();
	}

	void Application::Init(const AppProps& props)
	{
		// Subsystem pointers (is there a better way to do this?)
		m_Window			=	std::make_shared<Window>(props.WindowTitle, props.WindowWidth, props.WindowHeight);
		m_Events			=	std::make_shared<EventHandler>(m_Window);
		m_EntityManager		=	std::make_shared<EntityManager>();
		m_Input				=	std::make_shared<Input>();
		m_Assets			=	std::make_shared<Assets>();
		m_Systems			=	std::make_shared<Systems>(m_Window, m_EntityManager);
		m_Collision			=	m_Systems->GetCollision();
		m_Scene				=	std::make_shared<Scene>();
	}

	void Application::Shutdown()
	{
		for (auto& layer : m_Layers)
		{
			layer->OnDetach();
		}

		m_Window->Shutdown();
	}

	void Application::Run()
	{
		sf::Clock clock;

		while (m_Running)
		{
			auto time = clock.getElapsedTime().asSeconds();
			m_Timestep = Time::CalculateDeltaTime(time);

			m_Events->Listen();
			m_Input->SetEventList(m_Events->GetEvents());

			for (auto& layer : m_Layers)
			{
				layer->OnUpdate(m_Timestep);
			}

			m_EntityManager->Update();

			m_Scene->Update();

			m_Window->Clear();
			m_Systems->Run(m_Timestep);
			m_Window->Display();

			CheckWindowEvents();

			m_Events->Clear();

		}
	}

	void Application::CheckWindowEvents()
	{
		for (auto& [tag, entityVec] : m_Events->GetEvents())
		{
			if (tag == "window")
			{
				for (auto& e : entityVec)
				{
					if (e->eWindow->Type == "closed")
					{
						m_Running = false;
					}
					else if (e->eWindow->Type == "resized" && !e->m_Handled)
					{
						m_Window->SetSize(e->eWindow->Width, e->eWindow->Height);
					}

					e->m_Handled = true;
				}
			}
		}
	}

}