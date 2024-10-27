#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

#include "Core/ECB/Components/AComponent.h"
#include "Core/ECB/Components/DrawableComponent.h"

#include "Core/ECB/Behaviors/ABehavior.h"

enum class EGridEntityState : uint8_t
{
	DRAGGING = 1 << 0,
	MOVING   = 1 << 1,
};

namespace Core::ECB
{
	class Entity
	{
	public:
		Entity();
		virtual ~Entity() = default;

		virtual void SetPosition(const sf::Vector2f& p_position);

		virtual sf::Vector2f GetPosition() const;

		template <typename T, typename... Args>
		T* AddComponent(Args&&... p_args)
		{
			static_assert(std::is_base_of<Components::AComponent, T>::value, "T should derive from AComponent");

			if constexpr (std::is_base_of<Components::DrawableComponent, T>::value)
			{
				for (const auto& component : m_components)
				{
					if (dynamic_cast<Components::DrawableComponent*>(component.get()) != nullptr)
					{
						std::cerr << "Entity is already composed by a DrawableComponent.\n";
						return nullptr;
					}
				}
			}

			if (auto found = GetComponent<T>(); !found)
			{
				m_components.insert(m_components.begin(), std::make_shared<T>(*this, p_args...));
				T* instance = dynamic_cast<T*>(m_components.front().get());

				return instance;
			}
			else
			{
				return found;
			}
		}

		template <typename T>
		void RemoveComponent()
		{
			std::shared_ptr<T> result(nullptr);

			for (auto it = m_components.begin(); it != m_components.end(); ++it)
			{
				result = std::dynamic_pointer_cast<T>(*it);
				if (result)
				{
					m_components.erase(it);
				}
			}
		}

		template <typename T>
		T* GetComponent()
		{
			static_assert(std::is_base_of<Components::AComponent, T>::value, "T should derive from AComponent");

			std::shared_ptr<T> result(nullptr);

			for (auto it = m_components.begin(); it != m_components.end(); ++it)
			{
				result = std::dynamic_pointer_cast<T>(*it);
				if (result)
				{
					return result.get();
				}
			}

			return nullptr;
		}

		template <typename T, typename... Args>
		T& AddBehavior(Args&&... p_args)
		{
			static_assert(std::is_base_of<Behaviors::ABehavior, T>::value, "T should derive from ABehavior");

			if (auto found = GetBehavior<T>(); !found)
			{
				m_behaviors.insert(m_behaviors.begin(), std::make_shared<T>(*this, p_args...));
				T& instance = *dynamic_cast<T*>(m_behaviors.front().get());

				return instance;
			}
			else
			{
				return *found;
			}
		}

		template <typename T>
		T* GetBehavior()
		{
			static_assert(std::is_base_of<Behaviors::ABehavior, T>::value, "T should derive from ABehavior");

			std::shared_ptr<T> result(nullptr);

			for (auto it = m_behaviors.begin(); it != m_behaviors.end(); ++it)
			{
				result = std::dynamic_pointer_cast<T>(*it);
				if (result)
				{
					return result.get();
				}
			}

			return nullptr;
		}

		template <typename T>
		void RemoveBehavior()
		{
			std::shared_ptr<T> result(nullptr);

			for (auto it = m_behaviors.begin(); it != m_behaviors.end(); ++it)
			{
				result = std::dynamic_pointer_cast<T>(*it);
				if (result)
				{
					(*it)->OnDestroy();
					m_behaviors.erase(it);
				}
			}
		}

		void StartBehaviors() const
		{
			for (auto& behavior : m_behaviors)
			{
				if (!behavior->IsStarted())
				{
					behavior->Start();
					behavior->SetStarted(true);
				}
			}
		}

		void UpdateBehaviors(float deltaTime) const
		{
			for (auto& behavior : m_behaviors)
			{
				behavior->Update(deltaTime);
			}
		}

		void Update(float deltaTime) const
		{
			UpdateBehaviors(deltaTime);
		}

	private:
		std::vector<std::shared_ptr<Components::AComponent>> m_components;
		std::vector<std::shared_ptr<Behaviors::ABehavior>> m_behaviors;

		sf::Vector2f m_position;
	};
}
