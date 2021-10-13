// (c) 2021 JUV Studios. All rights reserved. Included as part of OneToolkit for use in C++ projects targeting the UWP XAML framework.

#pragma once
#include <OneToolkit.h>
#ifdef __cplusplus_winrt

#define DeclareDependencyProperty(Type, Name) private: static ::Windows::UI::Xaml::DependencyProperty^ m_##Name##Property;\
public: property Type Name { Type get(); void set(Type value); } static property ::Windows::UI::Xaml::DependencyProperty^ Name##Property { ::Windows::UI::Xaml::DependencyProperty^ get(); }

#define DefineDependencyProperty(Type, OwnerType, Name, DefaultValue) ::Windows::UI::Xaml::DependencyProperty^ OwnerType::m_##Name##Property = ::Windows::UI::Xaml::DependencyProperty::Register(#Name, ::juv::remove_hat<Type>::typeid, OwnerType::typeid,\
ref new ::Windows::UI::Xaml::PropertyMetadata(DefaultValue, ref new ::Windows::UI::Xaml::PropertyChangedCallback(&OwnerType::DependencyPropertyChanged)));\
::Windows::UI::Xaml::DependencyProperty^ OwnerType::Name##Property::get() { return m_##Name##Property; }\
Type OwnerType::Name::get() { return ::OneToolkit::UI::Xaml::GetValue<Type>(this, m_##Name##Property); } void OwnerType::Name::set(Type value) { SetValue(m_##Name##Property, value); }

namespace OneToolkit::UI::Xaml
{
	/// <summary>
	/// Gets the value of a DependencyProperty as the specified type.
	/// </summary>
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
#include <winrt/OneToolkit.UI.Xaml.Controls.h>
#include <winrt/OneToolkit.UI.Xaml.Triggers.h>
#include <winrt/OneToolkit.UI.Xaml.Media.Animation.h>

namespace winrt
{
	namespace OneToolkit::UI::Xaml
	{
		template <typename T = Windows::Foundation::IInspectable>
		auto GetValue(Windows::UI::Xaml::DependencyObject const& dependencyObject, Windows::UI::Xaml::DependencyProperty const& dependencyProperty)
		{
			return unbox_value<T>(dependencyObject.GetValue(dependencyProperty));
		}
	}
}

#endif