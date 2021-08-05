#pragma once
#include "Runtime.TypeNameHelper.g.h"

namespace winrt::OneToolkit::Runtime
{
	namespace implementation
	{
		struct TypeNameHelper : TypeNameHelperT<TypeNameHelper>
		{
		public:
			TypeNameHelper() = delete;
			static bool IsTypePrimitive(Windows::UI::Xaml::Interop::TypeName typeId);
			static hstring ExtractNamespace(Windows::UI::Xaml::Interop::TypeName typeId);
			static hstring ToString(Windows::UI::Xaml::Interop::TypeName typeId);
			static Windows::UI::Xaml::Interop::TypeName FromString(hstring const& typeName);
		private:
			static bool IsTypePrimitive(hstring const& typeName);
			static hstring ExtractNamespace(hstring const& typeName);
		};
	}

	namespace factory_implementation
	{
		struct TypeNameHelper : TypeNameHelperT<TypeNameHelper, implementation::TypeNameHelper>
		{
		};
	}
}