#pragma once
#include "Runtime.Reflection.g.h"

namespace winrt::OneToolkit::Runtime
{
	namespace implementation
	{
		struct Reflection : ReflectionT<Reflection>
		{
			Reflection() = delete;
			static IInspectable ActivateType(Windows::UI::Xaml::Interop::TypeName typeId);
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