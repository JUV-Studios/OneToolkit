#pragma once
#include "Lifecycle.WeakTypeCache.g.h"

namespace winrt::OneToolkit::Lifecycle
{
	namespace implementation
	{
		struct WeakTypeCache : WeakTypeCacheT<WeakTypeCache>
		{
		public:
			bool IsTypePresent(Windows::UI::Xaml::Interop::TypeName typeId);
			IInspectable GetInstance(Windows::UI::Xaml::Interop::TypeName typeId);
			IInspectable GetInstance(Windows::UI::Xaml::Interop::TypeName typeId, SingletonInstanceProvider const& provider);
			void Emplace(Windows::UI::Xaml::Interop::TypeName typeId, IInspectable const& instance);
			void Remove(Windows::UI::Xaml::Interop::TypeName typeId);
		private:
			slim_mutex m_AccessLock;
			std::map<hstring, weak_ref<IInspectable>> m_TypeCache;
		};
	}

	namespace factory_implementation
	{
		struct WeakTypeCache : WeakTypeCacheT<WeakTypeCache, implementation::WeakTypeCache>
		{
		};
	}
}