#pragma once
#ifdef __cplusplus_winrt
#include <type_traits>

namespace juv
{
	template <typename T>
	struct remove_hat 
	{ 
		using type = T;
	};

	template <typename T>
	struct remove_hat<T^> 
	{
		using type = typename remove_hat<T>::type;
	};
}

namespace WUXC = Windows::UI::Xaml::Controls;
namespace MUXC = Microsoft::UI::Xaml::Controls;

#define DeclareDependencyProperty(Type, Name) private: static ::Windows::UI::Xaml::DependencyProperty^ m_##Name##Property;\
public: property Type Name { Type get(); void set(Type value); } static property ::Windows::UI::Xaml::DependencyProperty^ Name##Property { ::Windows::UI::Xaml::DependencyProperty^ get(); }

#define DefineDependencyProperty(Type, OwnerType, Name, DefaultValue) ::Windows::UI::Xaml::DependencyProperty^ OwnerType::m_##Name##Property = ::Windows::UI::Xaml::DependencyProperty::Register(#Name, ::juv::remove_hat<Type>::typeid, OwnerType::typeid,\
ref new ::Windows::UI::Xaml::PropertyMetadata(DefaultValue, ref new ::Windows::UI::Xaml::PropertyChangedCallback(&OwnerType::DependencyPropertyChanged)));\
::Windows::UI::Xaml::DependencyProperty^ OwnerType::Name##Property::get() { return m_##Name##Property; }\
Type OwnerType::Name::get() { return ::OneToolkit::UI::Xaml::GetValue<Type>(this, m_##Name##Property); } void OwnerType::Name::set(Type value) { SetValue(m_##Name##Property, value); }

namespace OneToolkit::UI::Xaml
{
	template <typename T = Platform::Object>
	auto GetValue(Windows::UI::Xaml::DependencyObject^ dependencyObject, Windows::UI::Xaml::DependencyProperty^ dependencyProperty)
	{
		if constexpr (std::is_same_v<T, Platform::Object>) return dependencyObject->GetValue(dependencyProperty);
		else if constexpr (std::is_same_v<T, Platform::String>) return dependencyObject->GetValue(dependencyProperty)->ToString();
		else if constexpr (__is_ref_class(T) || __is_interface_class(T) || __is_ref_array(T) || __is_delegate(T)) return dynamic_cast<T^>(dependencyObject->GetValue(dependencyProperty));
		else return static_cast<T>(dependencyObject->GetValue(dependencyProperty));
	}
}

#else
#include <OneToolkit.h>
#include <winrt/Windows.UI.Xaml.Automation.Text.h>
#include <winrt/Windows.UI.Xaml.Automation.Peers.h>
#include <winrt/Windows.UI.Xaml.Automation.Provider.h>
#include <winrt/Windows.UI.Xaml.Controls.Maps.h>
#include <winrt/Windows.UI.Xaml.Controls.Primitives.h>
#include <winrt/Windows.UI.Xaml.Core.Direct.h>
#include <winrt/Windows.UI.Xaml.Documents.h>
#include <winrt/Windows.UI.Xaml.Hosting.h>
#include <winrt/Windows.UI.Xaml.Input.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Windows.UI.Xaml.Markup.h>
#include <winrt/Windows.UI.Xaml.Media.Animation.h>
#include <winrt/Windows.UI.Xaml.Media.Imaging.h>
#include <winrt/Windows.UI.Xaml.Media.Media3D.h>
#include <winrt/Windows.UI.Xaml.Navigation.h>
#include <winrt/Windows.UI.Xaml.Printing.h>
#include <winrt/Windows.UI.Xaml.Resources.h>
#include <winrt/Windows.UI.Xaml.Shapes.h>
#include <winrt/Microsoft.UI.Xaml.Automation.Peers.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <winrt/Microsoft.UI.Xaml.Controls.AnimatedVisuals.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Xaml.XamlTypeInfo.h>
#include <winrt/OneToolkit.UI.Xaml.Controls.h>
#include <winrt/OneToolkit.UI.Xaml.Converters.h>
#include <winrt/OneToolkit.UI.Xaml.Media.Animation.h>

namespace winrt
{
	namespace WUXC = Windows::UI::Xaml::Controls;
	namespace MUXC = Microsoft::UI::Xaml::Controls;
}

#endif