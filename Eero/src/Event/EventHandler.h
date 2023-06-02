#pragma once

#include <vector>
#include <string>
#include <map>

#include "Event.h"
#include "Window/Window.h"

namespace Eero {

	class EventHandler
	{
	public:
		EventHandler(std::shared_ptr<Window>& window);

		void Listen();
		void Clear();

		std::map<std::string, std::vector<std::shared_ptr<Event>>>& GetEvents() { return m_Events; }
	private:
		std::shared_ptr<Window> m_Window;
		std::map<std::string, std::vector<std::shared_ptr<Event>>> m_Events;
	};

}
