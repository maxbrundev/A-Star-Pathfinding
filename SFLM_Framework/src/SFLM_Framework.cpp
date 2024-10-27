#include <SFML/Graphics.hpp>

#include "Application.h"
#include "Context/Window.h"
#include "Inputs/InputManager.h"
#include "Tools/Time/Clock.h"

int main()
{
	Context::Settings::WindowSettings windowSettings;
	windowSettings.Title = "SFML_Framework";
	windowSettings.Width  = 800;
	windowSettings.Height = 600;

	Context::Window window(windowSettings);
	window.SetVSync(true);
	window.setMaxFrameRate(144);

	Inputs::InputManager input(window);

	Core::Application application(window, input);

	Tools::Time::Clock clock;

	while (window.IsActive())
	{
		window.PollEvents();
		window.GetWindow()->clear();

		application.Update(clock.GetDeltaTime());
		application.Render();

		window.GetWindow()->display();

		input.ClearEvents();

		clock.Update();
	}

	return 0;
}
