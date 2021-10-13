#include "pch.h"
#include "TypeCache.h"
#include "Lifecycle.TypeCache.g.cpp"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Interop;

namespace winrt::OneToolkit::Lifecycle::implementation
{
	TypeCacheReference::TypeCacheReference() : m_Strong({})
	{
	}

	void TypeCacheReference::operator=(std::pair<IInspectable, TypeCacheKind> value) noexcept
	{
		if (value.second == TypeCacheKind::Weak)
		{
			m_IsWeak = true;
			m_Weak = value.first;
		}
		else
		{
			m_IsWeak = false;
			m_Strong = value.first;
		}
	}

	TypeCacheReference::operator IInspectable() const noexcept
	{
		return m_IsWeak ? m_Weak.get() : m_Strong;
	}

	TypeCacheReference::operator bool() const noexcept
	{
		return m_IsWeak ? static_cast<bool>(m_Weak) : static_cast<bool>(m_Strong);
	}

	TypeCacheReference::~TypeCacheReference() noexcept
	{
		m_IsWeak ? m_Weak.~weak_ref() : m_Strong.~IInspectable();
	}

	TypeCache::TypeCache(TypeCacheKind kind) : m_Kind(kind)
	{
	}

	bool TypeCache::IsTypePresent(TypeName typeId)
	{
		const slim_lock_guard lockGuard{ m_AccessLock };
		return m_Storage[typeId.Name];
	}

	void TypeCache::Emplace(TypeName typeId, IInspectable const& instance)
	{
		const slim_lock_guard lockGuard{ m_AccessLock };
		m_Storage[typeId.Name] = { instance, m_Kind };
	}

	void TypeCache::Remove(TypeName typeId)
	{
		const slim_lock_guard lockGuard{ m_AccessLock };
		m_Storage.erase(typeId.Name);
	}

	IInspectable TypeCache::GetInstance(TypeName typeId, SingletonInstanceProvider instanceProvider)
	{
		const slim_lock_guard lockGuard{ m_AccessLock };
		auto& reference = m_Storage[typeId.Name];
		if (!reference && instanceProvider) reference = { instanceProvider(typeId), m_Kind };
		return reference;
	}
}