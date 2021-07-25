#include "pch.h"
#include "StrongTypeCache.h"
#include "Lifecycle.StrongTypeCache.g.cpp"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Interop;

namespace winrt::OneToolkit::Lifecycle::implementation
{
	bool StrongTypeCache::IsTypePresent(TypeName typeId)
	{
		const slim_lock_guard lockGuard { m_AccessLock };
		return m_TypeCache[typeId.Name] != nullptr;
	}

	IInspectable StrongTypeCache::GetInstance(TypeName typeId)
	{
		const slim_lock_guard lockGuard{ m_AccessLock };
		return m_TypeCache[typeId.Name];
	}

	IInspectable StrongTypeCache::GetInstance(TypeName typeId, SingletonInstanceProvider const& provider)
	{
		const slim_lock_guard lockGuard{ m_AccessLock };
		auto& value = m_TypeCache[typeId.Name];
		if (!value) value = provider(typeId);
		return value;
	}

	void StrongTypeCache::Emplace(TypeName typeId, IInspectable const& instance)
	{
		const slim_lock_guard lockGuard { m_AccessLock };
		m_TypeCache[typeId.Name] = instance;
	}

	void StrongTypeCache::Remove(TypeName typeId)
	{
		const slim_lock_guard lockGuard{ m_AccessLock };
		m_TypeCache.erase(typeId.Name);
	}
}