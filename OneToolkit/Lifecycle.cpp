#include "pch.h"
#include "Lifecycle.h"

using namespace juv;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml::Interop;

namespace winrt::OneToolkit::Lifecycle::implementation
{
	struct CacheMapIterator
	{
	public:
		CacheMapIterator(std::map<hstring, CacheValueReference>& storage) : m_Iterator(storage.begin()), m_Storage(storage) {}

		IInspectable Current() const noexcept
		{
			return m_Iterator->second;
		}

		bool HasCurrent() const noexcept
		{
			return m_Iterator != m_Storage.end();
		}

		uint32 GetMany(winrt::array_view<IKeyValuePair<hstring, IInspectable>> items)
		{
			throw hresult_not_implemented();
		}

		bool MoveNext() noexcept
		{
			if (m_Iterator != m_Storage.end())
			{
				m_Iterator = std::next(m_Iterator);
				return true;
			}
			
			return false;
		}
	private:
		std::map<hstring, CacheValueReference>& m_Storage;
		std::map<hstring, CacheValueReference>::const_iterator m_Iterator;
	};

	CacheValueReference::CacheValueReference() : m_Strong({}), m_Kind(0)
	{
	}

	void CacheValueReference::operator=(std::pair<IInspectable, ObjectReferenceKind> value) noexcept
	{
		m_Kind = static_cast<uint8>(value.second);
		switch (value.second)
		{
		case ObjectReferenceKind::Weak:
			m_Weak = value.first;
			break;
		case ObjectReferenceKind::Strong:
			m_Strong = value.first;
			break;
		case ObjectReferenceKind::AgileStrong:
			m_AgileStrong = value.first;
			break;
		}
	}

	CacheValueReference::operator IInspectable() const noexcept
	{
		switch (m_Kind)
		{
		case static_cast<uint8>(ObjectReferenceKind::Weak): return m_Weak.get();
		case static_cast<uint8>(ObjectReferenceKind::Strong): return m_Strong;
		case static_cast<uint8>(ObjectReferenceKind::AgileStrong): return m_AgileStrong.get();
		default: return nullptr;
		}
	}

	CacheValueReference::operator bool() const noexcept
	{
		switch (m_Kind)
		{
		case static_cast<uint8>(ObjectReferenceKind::Weak): return static_cast<bool>(m_Weak);
		case static_cast<uint8>(ObjectReferenceKind::Strong): return static_cast<bool>(m_Strong);
		case static_cast<uint8>(ObjectReferenceKind::AgileStrong): return static_cast<bool>(m_AgileStrong);
		default: return false;
		}
	}

	CacheValueReference::~CacheValueReference() noexcept
	{
		switch (m_Kind)
		{
		case static_cast<uint8>(ObjectReferenceKind::Weak): 
			m_Weak.~weak_ref();
			break;
		case static_cast<uint8>(ObjectReferenceKind::Strong):
			m_Strong.~IInspectable();
			break;
		case static_cast<uint8>(ObjectReferenceKind::AgileStrong):
			m_AgileStrong.~agile_ref();
			break;
		}
	}
}