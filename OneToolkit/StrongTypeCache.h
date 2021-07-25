#pragma once
#include "Lifecycle.StrongTypeCache.g.h"

namespace winrt::OneToolkit::Lifecycle
{
	namespace implementation
	{
		struct StrongTypeCache : StrongTypeCacheT<StrongTypeCache>
		{
		public:
			bool IsTypePresent(Windows::UI::Xaml::Interop::TypeName typeId);
			IInspectable GetInstance(Windows::UI::Xaml::Interop::TypeName typeId);
			IInspectable GetInstance(Windows::UI::Xaml::Interop::TypeName typeId, SingletonInstanceProvider const& provider);
			void Emplace(Windows::UI::Xaml::Interop::TypeName typeId, IInspectable const& instance);
			void Remove(Windows::UI::Xaml::Interop::TypeName typeId);
		private:
			slim_mutex m_AccessLock;
			std::map<hstring, IInspectable> m_TypeCache;
		};
	}

	namespace factory_implementation
	{
		struct StrongTypeCache : StrongTypeCacheT<StrongTypeCache, implementation::StrongTypeCache>
		{
		};
	}
}