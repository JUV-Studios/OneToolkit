#include "pch.h"
#include "TypeCache.h"
#include "Lifecycle.WeakTypeCache.g.cpp"
#include "Lifecycle.StrongTypeCache.g.cpp"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Interop;

namespace winrt::OneToolkit::Lifecycle::implementation
{
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
}