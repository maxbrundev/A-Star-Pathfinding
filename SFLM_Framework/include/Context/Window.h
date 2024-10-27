#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "Eventing/Event.h"
#include "Settings/WindowSettings.h"
#include "Settings/ECursorMode.h"
#include "Settings/ECursorVisibility.h"

namespace Context
{
	class Window
	{
	public:
		Eventing::Event<int> KeyPressedEvent;
		Eventing::Event<int> KeyReleasedEvent;
		Eventing::Event<int> MouseButtonPressedEvent;
		Eventing::Event<int> MouseButtonReleasedEvent;
		Eventing::Event<std::pair<int, int>> MouseMovedEvent;
		Eventing::Event<float> MouseWheelEvent;

		Eventing::Event<> CloseEvent;

		Eventing::Event<uint16_t, uint16_t> ResizeEvent;

		Window(const Settings::WindowSettings& p_windowSettings);
		~Window();

		void Draw(const sf::Drawable& p_drawable, const sf::RenderStates& p_states = sf::RenderStates::Default) const;
		void SetIcon(const sf::Image& p_icon, uint16_t p_width, uint16_t p_height) const;
		void PollEvents();
		void SetView(const sf::View& p_view) const;
		void SetVSync(bool p_value) const;
		void setMaxFrameRate(unsigned int p_value) const;
		void SetVisible(bool p_value) const;
		void SetClose();
		void SetCursorVisibility(Settings::ECursorVisibility p_cursorVisibility);
		void SetCursorMode(Settings::ECursorMode p_cursorMode);
		void SetSize(uint16_t p_width, uint16_t p_height);
		void SetPosition(int p_x, int p_y) const;

		std::pair<uint16_t, uint16_t> GetSize() const;
		sf::Vector2u GetRenderSize() const;
		float GetAspectRatio() const;
		Settings::ECursorVisibility GetCursorVisibility() const;
		Settings::ECursorMode GetCursorMode() const;
		sf::RenderWindow* GetWindow() const;
		sf::View GetView() const;

		bool IsActive() const;
		bool IsFullscreen() const;

	private:
		void CreateWindow();

	private:
		sf::RenderWindow* m_renderWindow;
		const sf::View* m_view;

		std::string m_title;

		float m_aspectRatio;
		uint32_t m_flags;

		bool m_isFullscreen;
		bool m_isActive;
		bool m_titleBar;
		bool m_resizable;
		bool m_closable;

		Settings::ECursorVisibility m_cursorVisibility;
		Settings::ECursorMode m_cursorMode;

		std::pair<uint16_t, uint16_t> m_size;
	};
}
