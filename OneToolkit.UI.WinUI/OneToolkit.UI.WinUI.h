// (c) 2021 JUV Studios. All rights reserved. Included as part of OneToolkit for use in C++ projects targeting the WinUI 3 framework.

#pragma once
#include <OneToolkit.h>
#ifdef __cplusplus_winrt

#define DeclareDependencyProperty(Type, Name) private: static ::Microsoft::UI::Xaml::DependencyProperty^ m_##Name##Property;\
public: property Type Name { Type get(); void set(Type value); } static property ::Microsoft::UI::Xaml::DependencyProperty^ Name##Property { ::Microsoft::UI::Xaml::DependencyProperty^ get(); }

#define DefineDependencyProperty(Type, OwnerType, Name, DefaultValue) ::Microsoft::UI::Xaml::DependencyProperty^ OwnerType::m_##Name##Property = ::Microsoft::UI::Xaml::DependencyProperty::Register(#Name, ::juv::remove_hat<Type>::typeid, OwnerType::typeid,\
ref new ::Microsoft::UI::Xaml::PropertyMetadata(DefaultValue, ref new ::Microsoft::UI::Xaml::PropertyChangedCallback(&OwnerType::DependencyPropertyChanged)));\
::Microsoft::UI::Xaml::DependencyProperty^ OwnerType::Name##Property::get() { return m_##Name##Property; }\
Type OwnerType::Name::get() { return ::OneToolkit::UI::Xaml::GetValue<Type>(this, m_##Name##Property); } void OwnerType::Name::set(Type value) { SetValue(m_##Name##Property, value); }

namespace OneToolkit::UI::Xaml
{
	/// <summary>
	/// Gets the value of a DependencyProperty as the specified type.
	/// </summary>
	template <typename T = Platform::Object>
	auto GetValue(Microsoft::UI::Xaml::DependencyObject^ dependencyObject, Microsoft::UI::Xaml::DependencyProperty^ dependencyProperty)
	{
		if constexpr (std::is_same_v<T, Platform::Object>) return dependencyObject->GetValue(dependencyProperty);
		else if constexpr (std::is_same_v<T, Platform::String>) return dependencyObject->GetValue(dependencyProperty)->ToString();
		else if constexpr (__is_ref_class(T) || __is_interface_class(T) || __is_ref_array(T) || __is_delegate(T)) return dynamic_cast<T^>(dependencyObject->GetValue(dependencyProperty));
		else return static_cast<T>(dependencyObject->GetValue(dependencyProperty));
	}
}

#else
#endif