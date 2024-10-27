#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>

#include "ECB/Components/AComponent.h"

class CumulativeTags
{
public:
	CumulativeTags() : m_tagNumber(0), m_bitField(0)
	{
	}

	void Initialize(int p_tagCount)
	{
		m_tagNumber = p_tagCount;
		m_bitField = 0;
		m_tagsCount.assign(m_tagNumber, 0);
	}

	void AddTags(int p_mask)
	{
		for (int i = 0; i < m_tagNumber; ++i)
		{
			if (p_mask & (1 << i))
			{
				m_tagsCount[i]++;
			}
		}
		m_bitField |= p_mask;
	}

	void AddUniqueTags(int p_mask)
	{
		for (int i = 0; i < m_tagNumber; ++i)
		{
			if (p_mask & (1 << i))
			{
				m_tagsCount[i] = 1;
			}
		}
		m_bitField |= p_mask;
	}

	void RemoveTags(int p_mask)
	{
		for (int i = 0; i < m_tagNumber; ++i)
		{
			int bit = 1 << i;

			if (p_mask & bit)
			{
				m_tagsCount[i]--;

				if (m_tagsCount[i] <= 0)
				{
					m_bitField &= ~bit;

					if (m_tagsCount[i] < 0)
					{
						m_tagsCount[i] = 0;
					}
				}
			}
		}
	}

	void ClearTags(int p_mask)
	{
		for (int i = 0; i < m_tagNumber; ++i)
		{
			int bit = 1 << i;

			if (p_mask & bit)
			{
				m_tagsCount[i] = 0;
				m_bitField &= ~bit;
			}
		}
	}

	void FlushAllTags()
	{
		std::fill(m_tagsCount.begin(), m_tagsCount.end(), 0);
		m_bitField = 0;
	}

	bool HasAllTags(int p_mask) const
	{
		return (m_bitField & p_mask) == p_mask;
	}

	bool HasAnyTags(int p_mask) const
	{
		return (m_bitField & p_mask) != 0;
	}

private:
	std::vector<int> m_tagsCount;
	int m_tagNumber;
	int m_bitField;
};

namespace Core::ECB::Components
{
	template <typename T>
	concept EnumType = std::is_enum_v<T>;

	class TagHandlerComponent : public AComponent
	{
	public:
		TagHandlerComponent(Entity& p_owner) : AComponent(p_owner)
		{
		}

		template <typename EnumType>
		void InitTagSet()
		{
			auto typeIndex = std::type_index(typeid(EnumType));

			if (m_tagSets.find(typeIndex) == m_tagSets.end())
			{
				int numTags = sizeof(EnumType) * 8;
				m_tagSets[typeIndex] = std::make_unique<CumulativeTags>();
				m_tagSets[typeIndex]->Initialize(numTags);
			}
		}

		template <typename EnumType>
		void AddTags(EnumType p_tag)
		{
			auto typeIdx = std::type_index(typeid(EnumType));

			if (m_tagSets.find(typeIdx) != m_tagSets.end())
			{
				m_tagSets[typeIdx]->AddTags(static_cast<int>(p_tag));
			}
		}

		template <typename EnumType>
		void AddUniqueTags(EnumType p_tag)
		{
			auto typeIdx = std::type_index(typeid(EnumType));
			if (m_tagSets.find(typeIdx) != m_tagSets.end())
			{
				m_tagSets[typeIdx]->AddUniqueTags(static_cast<int>(p_tag));
			}
		}

		template <typename EnumType>
		void RemoveTags(EnumType p_tag)
		{
			auto typeIdx = std::type_index(typeid(EnumType));
			if (m_tagSets.find(typeIdx) != m_tagSets.end())
			{
				m_tagSets[typeIdx]->RemoveTags(static_cast<int>(p_tag));
			}
		}

		template <typename EnumType>
		bool HasAllTags(EnumType p_tag) const
		{
			auto typeIdx = std::type_index(typeid(EnumType));
			if (m_tagSets.find(typeIdx) != m_tagSets.end())
			{
				return m_tagSets.at(typeIdx)->HasAllTags(static_cast<int>(p_tag));
			}
			return false;
		}

		template <typename EnumType>
		bool HasAnyTags(EnumType p_tag) const
		{
			auto typeIdx = std::type_index(typeid(EnumType));
			if (m_tagSets.find(typeIdx) != m_tagSets.end())
			{
				return m_tagSets.at(typeIdx)->HasAnyTags(static_cast<int>(p_tag));
			}
			return false;
		}

		template <typename EnumType>
		void ClearTags(EnumType p_tag)
		{
			auto typeIdx = std::type_index(typeid(EnumType));
			if (m_tagSets.find(typeIdx) != m_tagSets.end())
			{
				m_tagSets[typeIdx]->ClearTags(static_cast<int>(p_tag));
			}
		}

		void FlushAllTags() const
		{
			for (auto& tagSet : m_tagSets)
			{
				tagSet.second->FlushAllTags();
			}
		}

	private:
		std::unordered_map<std::type_index, std::unique_ptr<CumulativeTags>> m_tagSets;
	};
}
