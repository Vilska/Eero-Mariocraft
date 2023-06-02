#pragma once

#include "Core/Log.h"

extern std::shared_ptr<Eero::Application> Eero::CreateApplication();

int main(int argc, char** argv)
{
	Eero::Log::Init();
	LOG_INFO("Logger ready!");

	auto app = Eero::CreateApplication();
	app->Run();
}
