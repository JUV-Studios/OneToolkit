#pragma once
#if __has_include(<winrt/Microsoft.UI.h>)
#define USE_WINUI3
#include <winrt/Microsoft.UI.Xaml.h>

namespace winrt
{
	namespace XamlFramework = Microsoft::UI::Xaml;
}

#else
#include <winrt/Windows.UI.Xaml.h>
#if __has_include(<winrt/Microsoft.UI.Xaml.h>)
#define USE_WINUI2
#include <winrt/Microsoft.UI.Xaml.h>
#else
#define USE_PLATFORM_XAML
#endif

namespace winrt
{
	namespace XamlFramework = Windows::UI::Xaml;	
}

#endif