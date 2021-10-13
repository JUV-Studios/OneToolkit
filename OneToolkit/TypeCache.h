#pragma once
#include "Lifecycle.TypeCache.g.h"

namespace winrt::OneToolkit::Lifecycle
{
	namespace implementation
	{
		struct TypeCacheReference
		{
		public:
			TypeCacheReference();
			void operator=(std::pair<Windows::Foundation::IInspectable, TypeCacheKind> value) noexcept;
			operator Windows::Foundation::IInspectable() const noexcept;
			operator bool() const noexcept;
			~TypeCacheReference() noexcept;
		private:
			bool m_IsWeak = false;
			union
			{
				Windows::Foundation::IInspectable m_Strong;
				weak_ref<Windows::Foundation::IInspectable> m_Weak;
			};
		};

		struct TypeCache : TypeCacheT<TypeCache>
		{
		public:
			TypeCache(TypeCacheKind kind);
			DeclareAutoProperty(TypeCacheKind, Kind, {});
			bool IsTypePresent(Windows::UI::Xaml::Interop::TypeName typeId);
			void Emplace(Windows::UI::Xaml::Interop::TypeName typeId, IInspectable const& instance);
			void Remove(Windows::UI::Xaml::Interop::TypeName typeId);
			IInspectable GetInstance(Windows::UI::Xaml::Interop::TypeName typeId, SingletonInstanceProvider instanceProvider = nullptr);
		private:
			slim_mutex m_AccessLock;
			std::map<hstring, TypeCacheReference> m_Storage;
		};
	}

	namespace factory_implementation
	{
		struct TypeCache : TypeCacheT<TypeCache, implementation::TypeCache>
		{
		};
	}
}