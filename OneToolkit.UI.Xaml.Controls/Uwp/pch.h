#pragma once
#include <unknwn.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.UI.Xaml.h>
#include <winrt/Windows.UI.Xaml.Controls.h>
#include <winrt/Windows.UI.Xaml.Controls.Primitives.h>
#include <winrt/Windows.UI.Xaml.Data.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Windows.UI.Xaml.Markup.h>
#include <winrt/Windows.UI.Xaml.Navigation.h>
#include "winrt/Microsoft.UI.Xaml.Automation.Peers.h"
#include "winrt/Microsoft.UI.Xaml.Controls.Primitives.h"
#include "winrt/Microsoft.UI.Xaml.Media.h"
#include "winrt/Microsoft.UI.Xaml.XamlTypeInfo.h"
#include "Base.h"

#define DeclareDependencyProperty(Type, Name) private: static ::winrt::XamlFramework::DependencyProperty const s_##Name##Property;\
public: static ::winrt::XamlFramework::DependencyProperty Name##Property() noexcept\
{\
	return s_##Name##Property;\
}\
\
Type Name() const\
{\
	return ::winrt::unbox_value<Type>(GetValue(s_##Name##Property));\
}\
\
void Name(Type const& value)\
{\
	SetValue(s_##Name##Property, ::winrt::box_value(value));\
}