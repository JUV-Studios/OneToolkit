#include "pch.h"
#include "WeakTypeCache.h"
#include "Lifecycle.WeakTypeCache.g.cpp"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Interop;

namespace winrt::OneToolkit::Lifecycle::implementation
{
	bool WeakTypeCache::IsTypePresent(TypeName typeId)
	{
		const slim_lock_guard lockGuard { m_AccessLock };
		return m_TypeCache[typeId.Name].get() != nullptr;
	}

	IInspectable WeakTypeCache::GetInstance(TypeName typeId)
	{
		const slim_lock_guard lockGuard{ m_AccessLock };
		return m_TypeCache[typeId.Name].get();
	}

	IInspectable WeakTypeCache::GetInstance(TypeName typeId, SingletonInstanceProvider const& provider)
	{
		const slim_lock_guard lockGuard{ m_AccessLock };
		auto& value = m_TypeCache[typeId.Name];
		auto reference = value.get();
		if (!reference)
		{
			reference = provider(typeId);
			value = reference;
		}

		return reference;
	}

	void WeakTypeCache::Emplace(TypeName typeId, IInspectable const& instance)
	{
		const slim_lock_guard lockGuard{ m_AccessLock };
		m_TypeCache[typeId.Name] = instance;
	}

	void WeakTypeCache::Remove(TypeName typeId)
	{
		const slim_lock_guard lockGuard{ m_AccessLock };
		m_TypeCache.erase(typeId.Name);
	}
}