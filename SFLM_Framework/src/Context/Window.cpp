#include "Context/Window.h"

#include <SFML/Window/Event.hpp>

Context::Window::Window(const Settings::WindowSettings& p_windowSettings) :
m_renderWindow(nullptr),
m_title(p_windowSettings.Title),
m_aspectRatio(static_cast<float>(p_windowSettings.Width) / static_cast<float>(p_windowSettings.Height)),
m_flags(0),
m_isFullscreen(p_windowSettings.FullScreen),
m_isActive(false),
m_titleBar(p_windowSettings.TitleBar),
m_resizable(p_windowSettings.Resizable),
m_closable(p_windowSettings.Closable),
m_cursorVisibility(Settings::ECursorVisibility::ENABLED),
m_cursorMode(Settings::ECursorMode::FREE),
m_size{p_windowSettings.Width, p_windowSettings.Height}
{
	CreateWindow();
	m_isActive = true;
}

Context::Window::~Window()
{
	m_renderWindow->close();
	m_renderWindow = nullptr;
}

void Context::Window::Draw(const sf::Drawable& p_drawable, const sf::RenderStates& p_states) const
{
	m_renderWindow->draw(p_drawable, p_states);
}

void Context::Window::SetIcon(const sf::Image& p_icon, uint16_t p_width, uint16_t p_height) const
{
	m_renderWindow->setIcon(p_width, p_height, p_icon.getPixelsPtr());
}

void Context::Window::PollEvents()
{
	sf::Event event;

	while (m_renderWindow->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			KeyPressedEvent.Invoke(event.key.code);
			break;
		case sf::Event::KeyReleased:
			KeyReleasedEvent.Invoke(event.key.code);
			break;
		case sf::Event::MouseButtonPressed:
			MouseButtonPressedEvent.Invoke(event.mouseButton.button);
			break;
		case sf::Event::MouseButtonReleased:
			MouseButtonReleasedEvent.Invoke(event.mouseButton.button);
			break;
		case sf::Event::MouseMoved:
			MouseMovedEvent.Invoke(std::make_pair(event.mouseMove.x, event.mouseMove.y));
			break;
		case sf::Event::MouseWheelScrolled:
			MouseWheelEvent.Invoke(event.mouseWheelScroll.delta);
			break;
		case sf::Event::Closed:
			SetClose();
			break;
		}
	}
}

void Context::Window::SetView(const sf::View& p_view) const
{
	m_renderWindow->setView(p_view);
}

void Context::Window::SetVSync(bool p_value) const
{
	m_renderWindow->setVerticalSyncEnabled(p_value);
}

void Context::Window::setMaxFrameRate(unsigned int p_value) const
{
	m_renderWindow->setFramerateLimit(p_value);
}

void Context::Window::SetVisible(bool p_value) const
{
	m_renderWindow->setVisible(p_value);
}

void Context::Window::SetClose()
{
	m_renderWindow->close();
	m_isActive = false;
}

bool Context::Window::IsActive() const
{
	return m_isActive;
}

bool Context::Window::IsFullscreen() const
{
	return m_isFullscreen;
}

void Context::Window::SetCursorVisibility(Settings::ECursorVisibility p_cursorVisibility)
{
	m_cursorVisibility = p_cursorVisibility;
	m_renderWindow->setMouseCursorVisible(m_cursorVisibility == Settings::ECursorVisibility::ENABLED);
}

void Context::Window::SetCursorMode(Settings::ECursorMode p_cursorMode)
{
	m_cursorMode = p_cursorMode;
	m_renderWindow->setMouseCursorGrabbed(p_cursorMode == Settings::ECursorMode::GRABBED);
}

void Context::Window::SetSize(uint16_t p_width, uint16_t p_height)
{
	m_size.first = p_width;
	m_size.second = p_height;

	m_aspectRatio = static_cast<float>(p_width) / static_cast<float>(p_height);

	m_renderWindow->setSize(sf::Vector2u(m_size.first, m_size.second));
}

void Context::Window::SetPosition(int p_x, int p_y) const
{
	m_renderWindow->setPosition(sf::Vector2i(p_x, p_y));
}

std::pair<uint16_t, uint16_t> Context::Window::GetSize() const
{
	return m_size;
}

sf::Vector2u Context::Window::GetRenderSize() const
{
	return m_renderWindow->getSize();
}

float Context::Window::GetAspectRatio() const
{
	return m_aspectRatio;
}

Context::Settings::ECursorVisibility Context::Window::GetCursorVisibility() const
{
	return m_cursorVisibility;
}

Context::Settings::ECursorMode Context::Window::GetCursorMode() const
{
	return m_cursorMode;
}

sf::RenderWindow* Context::Window::GetWindow() const
{
	return m_renderWindow;
}

sf::View Context::Window::GetView() const
{
	return m_renderWindow->getView();
}

void Context::Window::CreateWindow()
{
	m_flags |= m_isFullscreen ? sf::Style::Fullscreen : 0;
	m_flags |= m_titleBar     ? sf::Style::Titlebar   : 0;
	m_flags |= m_resizable    ? sf::Style::Resize     : 0;
	m_flags |= m_closable     ? sf::Style::Close      : 0;

	m_renderWindow = new sf::RenderWindow();
	m_renderWindow->create(sf::VideoMode(m_size.first, m_size.second), m_title.c_str(), m_flags);
}
