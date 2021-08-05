#pragma once
#include "Lifecycle.WeakTypeCache.g.h"
#include "Lifecycle.StrongTypeCache.g.h"

namespace winrt::OneToolkit::Lifecycle
{
	namespace implementation
	{
		template <typename T>
		struct TypeCacheBase
		{
		public:
			bool IsTypePresent(Windows::UI::Xaml::Interop::TypeName typeId)
			{
				const slim_lock_guard lockGuard{ m_AccessLock };
				return static_cast<bool>(m_TypeCache[typeId.Name]);
			}

			void Emplace(Windows::UI::Xaml::Interop::TypeName typeId, Windows::Foundation::IInspectable const& instance)
			{
				const slim_lock_guard lockGuard{ m_AccessLock };
				m_TypeCache[typeId.Name] = instance;
			}

			void Remove(Windows::UI::Xaml::Interop::TypeName typeId)
			{
				const slim_lock_guard lockGuard{ m_AccessLock };
				m_TypeCache.erase(typeId.Name);
			}
		protected:
			slim_mutex m_AccessLock;
			std::map<hstring, T> m_TypeCache;
		};

		struct WeakTypeCache : WeakTypeCacheT<WeakTypeCache>, TypeCacheBase<weak_ref<Windows::Foundation::IInspectable>>
		{
			IInspectable GetInstance(Windows::UI::Xaml::Interop::TypeName typeId);
			IInspectable GetInstance(Windows::UI::Xaml::Interop::TypeName typeId, SingletonInstanceProvider const& provider);
		};

		struct StrongTypeCache : StrongTypeCacheT<StrongTypeCache>, TypeCacheBase<Windows::Foundation::IInspectable>
		{
			IInspectable GetInstance(Windows::UI::Xaml::Interop::TypeName typeId);
			IInspectable GetInstance(Windows::UI::Xaml::Interop::TypeName typeId, SingletonInstanceProvider const& provider);
		};
	}

	namespace factory_implementation
	{
		struct WeakTypeCache : WeakTypeCacheT<WeakTypeCache, implementation::WeakTypeCache>
		{
		};

		struct StrongTypeCache : StrongTypeCacheT<StrongTypeCache, implementation::StrongTypeCache>
		{
		};
	}
}