// (c) 2021 JUV Studios. All rights reserved. Included as part of OneToolkit for use in C++ projects with XAML.

#pragma once
#include <OneToolkit.h>
#ifdef __cplusplus_winrt
#define declare_dependency_property(Type, Name) private: static AppFramework::DependencyProperty^ m_##Name##Property;\
public: property Type Name { Type get(); void set(Type value); } static property AppFramework::DependencyProperty^ Name##Property { AppFramework::DependencyProperty^ get(); }
#define define_dependency_property(Type, OwnerType, Name, DefaultValue) AppFramework::DependencyProperty^ OwnerType::m_##Name##Property = AppFramework::DependencyProperty::Register(#Name, ::Platform::Details::RemoveHat<Type>::type::typeid, OwnerType::typeid,\
ref new AppFramework::PropertyMetadata(DefaultValue, ref new AppFramework::PropertyChangedCallback(&OwnerType::DependencyPropertyChanged)));\
AppFramework::DependencyProperty^ OwnerType::Name##Property::get() { return m_##Name##Property; }\
Type OwnerType::Name::get() { return static_cast<Type>(this->GetValue(m_##Name##Property)); } void OwnerType::Name::set(Type value) { SetValue(m_##Name##Property, value); }
#else
#include <winrt/OneToolkit.UI.Xaml.Controls.h>
#include <winrt/OneToolkit.UI.Xaml.Triggers.h>
#endif