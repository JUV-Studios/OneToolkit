#pragma once
#ifdef _WIN32
#if __has_include(<winrt/Microsoft.UI.h>)
#define WINUI_VERSION 3
#include <winrt/Microsoft.UI.Xaml.h>
namespace winrt
{
	namespace XamlFramework = Microsoft::UI::Xaml;
}
#else
#include <winrt/Windows.UI.Xaml.h>
#if __has_include(<winrt/Microsoft.UI.Xaml.h>)
#define WINUI_VERSION 2
#include <winrt/Microsoft.UI.Xaml.h>
#else
#define WINUI_VERSION 1
#endif
namespace winrt
{
	namespace XamlFramework = Windows::UI::Xaml;
}
#endif
#endif