#pragma once
#include "Runtime.Reflection.g.h"

namespace winrt::OneToolkit::Runtime
{
	namespace implementation
	{
		struct Reflection : ReflectionT<Reflection>
		{
			Reflection() = delete;
			static Windows::UI::Xaml::Interop::TypeName GetTypeId(hstring const& typeName);
			static juv::uint64 GetSizeOf(Windows::UI::Xaml::Interop::TypeName typeId);
			static IInspectable ActivateType(Windows::UI::Xaml::Interop::TypeName typeId);
			static hstring ExtractNamespace(Windows::UI::Xaml::Interop::TypeName typeId);
			static hstring GetMetadataFileLocation(Windows::UI::Xaml::Interop::TypeName typeId);
		};
	}

	namespace factory_implementation
	{
		struct Reflection : ReflectionT<Reflection, implementation::Reflection>
		{
		};
	}
}