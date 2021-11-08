// (c) 2021 JUV Studios. All rights reserved. Included as part of OneToolkit for use in C++ projects targeting the UWP XAML framework.

#pragma once
#include <juv.h>
#ifdef __cplusplus_winrt

#define declare_dependency_property(Type, Name) private: static ::Windows::UI::Xaml::DependencyProperty^ m_##Name##Property;\
public: property Type Name { Type get(); void set(Type value); } static property ::Windows::UI::Xaml::DependencyProperty^ Name##Property { ::Windows::UI::Xaml::DependencyProperty^ get(); }

#define define_dependency_property(Type, OwnerType, Name, DefaultValue) ::Windows::UI::Xaml::DependencyProperty^ OwnerType::m_##Name##Property = ::Windows::UI::Xaml::DependencyProperty::Register(#Name, ::juv::remove_hat<Type>::typeid, OwnerType::typeid,\
ref new ::Windows::UI::Xaml::PropertyMetadata(DefaultValue, ref new ::Windows::UI::Xaml::PropertyChangedCallback(&OwnerType::DependencyPropertyChanged)));\
::Windows::UI::Xaml::DependencyProperty^ OwnerType::Name##Property::get() { return m_##Name##Property; }\
Type OwnerType::Name::get() { return static_cast<Type>(this->GetValue(m_##Name##Property)); } void OwnerType::Name::set(Type value) { SetValue(m_##Name##Property, value); }

#else
#include <winrt/OneToolkit.UI.Xaml.Controls.h>
#include <winrt/OneToolkit.UI.Xaml.Triggers.h>
#endif