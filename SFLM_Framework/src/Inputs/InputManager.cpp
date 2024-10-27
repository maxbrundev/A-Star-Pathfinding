#include "Inputs/InputManager.h"

#include <functional>

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

Inputs::InputManager::InputManager(Context::Window& p_window) : m_window(p_window)
{
	m_keyPressedListener = m_window.KeyPressedEvent.AddListener(std::bind(&InputManager::OnKeyPressed, this, std::placeholders::_1));
	m_keyReleasedListener = m_window.KeyReleasedEvent.AddListener(std::bind(&InputManager::OnKeyReleased, this, std::placeholders::_1));
	m_mouseButtonPressedListener = m_window.MouseButtonPressedEvent.AddListener(std::bind(&InputManager::OnMouseButtonPressed, this, std::placeholders::_1));
	m_mouseButtonReleasedListener = m_window.MouseButtonReleasedEvent.AddListener(std::bind(&InputManager::OnMouseButtonReleased, this, std::placeholders::_1));
	m_mouseMovedListener = m_window.MouseMovedEvent.AddListener(std::bind(&InputManager::OnMouseMoved, this, std::placeholders::_1));
	m_window.MouseWheelEvent.AddListener(std::bind(&InputManager::OnMouseWheel, this, std::placeholders::_1));
}

Inputs::InputManager::~InputManager()
{
	m_window.KeyPressedEvent.RemoveListener(m_keyPressedListener);
	m_window.KeyReleasedEvent.RemoveListener(m_keyReleasedListener);
	m_window.MouseButtonPressedEvent.RemoveListener(m_mouseButtonPressedListener);
	m_window.MouseButtonReleasedEvent.RemoveListener(m_mouseButtonReleasedListener);
	m_window.MouseMovedEvent.RemoveListener(m_mouseMovedListener);
}

Inputs::EKeyState Inputs::InputManager::GetKeyState(EKey p_key) const
{
	if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(p_key)))
		return EKeyState::DOWN;
	return EKeyState::UP;
}

Inputs::EMouseButtonState Inputs::InputManager::GetMouseButtonState(EMouseButton p_button) const
{
	if (sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(p_button)))
		return EMouseButtonState::MOUSE_DOWN;
	return EMouseButtonState::MOUSE_UP;
}

bool Inputs::InputManager::IsKeyPressed(EKey p_key) const
{
	return m_keyEvents.find(p_key) != m_keyEvents.end() && m_keyEvents.at(p_key) == EKeyState::DOWN;
}

bool Inputs::InputManager::IsKeyReleased(EKey p_key) const
{
	return m_keyEvents.find(p_key) != m_keyEvents.end() && m_keyEvents.at(p_key) == EKeyState::UP;
}

bool Inputs::InputManager::IsMouseButtonPressed(EMouseButton p_button) const
{
	return m_mouseButtonEvents.find(p_button) != m_mouseButtonEvents.end() && m_mouseButtonEvents.at(p_button) == EMouseButtonState::MOUSE_DOWN;
}

bool Inputs::InputManager::IsMouseButtonReleased(EMouseButton p_button) const
{
	return m_mouseButtonEvents.find(p_button) != m_mouseButtonEvents.end() && m_mouseButtonEvents.at(p_button) == EMouseButtonState::MOUSE_UP;
}

std::pair<int, int> Inputs::InputManager::GetMousePosition() const
{
	sf::Vector2i position = sf::Mouse::getPosition(*m_window.GetWindow());
	return std::make_pair(position.x, position.y);
}

std::pair<int, int> Inputs::InputManager::GetMouseRelativeMovement() const
{
	return m_mouseRelativeMovement;
}

int Inputs::InputManager::GetMouseWheel() const
{
	return m_mouseWheel;
}

void Inputs::InputManager::LockMouse() const
{
	m_window.SetCursorMode(Context::Settings::ECursorMode::GRABBED);
	m_window.SetCursorVisibility(Context::Settings::ECursorVisibility::DISABLED);
}

void Inputs::InputManager::UnlockMouse() const
{
	m_window.SetCursorMode(Context::Settings::ECursorMode::FREE);
	m_window.SetCursorVisibility(Context::Settings::ECursorVisibility::ENABLED);
}

bool Inputs::InputManager::IsMouseLocked() const
{
	return m_window.GetCursorMode() == Context::Settings::ECursorMode::GRABBED;
}

void Inputs::InputManager::ClearEvents()
{
	m_keyEvents.clear();
	m_mouseButtonEvents.clear();
}

void Inputs::InputManager::OnKeyPressed(int p_key)
{
	m_keyEvents[static_cast<EKey>(p_key)] = EKeyState::DOWN;
}

void Inputs::InputManager::OnKeyReleased(int p_key)
{
	m_keyEvents[static_cast<EKey>(p_key)] = EKeyState::UP;
}

void Inputs::InputManager::OnMouseButtonPressed(int p_button)
{
	m_mouseButtonEvents[static_cast<EMouseButton>(p_button)] = EMouseButtonState::MOUSE_DOWN;
}

void Inputs::InputManager::OnMouseButtonReleased(int p_button)
{
	m_mouseButtonEvents[static_cast<EMouseButton>(p_button)] = EMouseButtonState::MOUSE_UP;
}

void Inputs::InputManager::OnMouseMoved(std::pair<int, int> p_motion)
{
	m_mouseRelativeMovement = p_motion;
}

void Inputs::InputManager::OnMouseWheel(float p_wheel)
{
	m_mouseWheel = p_wheel;
}
