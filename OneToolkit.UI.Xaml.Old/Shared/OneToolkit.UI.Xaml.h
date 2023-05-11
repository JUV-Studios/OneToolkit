// (c) 2021 JUV Studios. All rights reserved. Included as part of OneToolkit for use in C++ projects with XAML.

#pragma once
#ifdef __cplusplus_winrt
#include "juv.h"
/*#ifdef framework_winui3
namespace XamlFramework = Microsoft::UI::Xaml;
#else */
namespace XamlFramework = Windows::UI::Xaml;
//#endif
#define declare_dependency_property(Type, Name) private: static XamlFramework::DependencyProperty^ m_##Name##Property;\
public: property Type Name { Type get(); void set(Type value); } static property XamlFramework::DependencyProperty^ Name##Property { XamlFramework::DependencyProperty^ get(); }
#define define_dependency_property(Type, OwnerType, Name, DefaultValue) XamlFramework::DependencyProperty^ OwnerType::m_##Name##Property = XamlFramework::DependencyProperty::Register(#Name, ::Platform::Details::RemoveHat<Type>::type::typeid, OwnerType::typeid,\
ref new XamlFramework::PropertyMetadata(DefaultValue, ref new XamlFramework::PropertyChangedCallback(&OwnerType::DependencyPropertyChanged)));\
XamlFramework::DependencyProperty^ OwnerType::Name##Property::get() { return m_##Name##Property; }\
Type OwnerType::Name::get() { return static_cast<Type>(this->GetValue(m_##Name##Property)); } void OwnerType::Name::set(Type value) { SetValue(m_##Name##Property, value); }

namespace OneToolkit::UI::Xaml
{
	template <typename T>
	T GetResourceValue(XamlFramework::ResourceDictionary^ , Platform::Object^ key, T defaultValue)
	{
		if (resources->HasKey(key))
		{
			try { return static_cast<T>(resources->Lookup(key)); }
			catch (Platform::InvalidCastException^) {}
		}

		return defaultValue;
	}

	enum class DependencyPropertyKind : uint8
	{
		Member, Attached
	};

	template <typename Value, typename Owner, DependencyPropertyKind Kind>
	struct DependencyPropertyWrapper
	{
		using ValueChangedHandler = void(*)(XamlFramework::DependencyObject^ dependencyObject, Value oldValue, Value newValue);

		DependencyPropertyWrapper(Platform::String^ propertyName, ValueChangedHandler valueChangedHandler, Value defaultValue = {}) : m_ValueChangedHandler(valueChangedHandler)
		{
			auto dependencyPropertyChanged = [this](XamlFramework::DependencyObject^ dependencyObject, XamlFramework::DependencyPropertyChangedEventArgs^ args)
			{
				if (args->Property == m_Property) m_ValueChangedHandler(dependencyObject, static_cast<Value>(args->OldValue), static_cast<Value>(args->NewValue));
			};

			if constexpr (Kind == DependencyPropertyKind::Member)
			{
				m_Property = XamlFramework::DependencyProperty::Register(propertyName, Value::typeid, Owner::typeid, ref new XamlFramework::PropertyMetadata(defaultValue, ref new PropertyChangedCallback(dependencyPropertyChanged)));
			}
			else
			{
				m_Property = XamlFramework::DependencyProperty::RegisterAttached(propertyName, Value::typeid, Owner::typeid, ref new XamlFramework::PropertyMetadata(defaultValue, ref new PropertyChangedCallback(dependencyPropertyChanged)));
			}
		}

		__declspec(property(get = get_Property)) XamlFramework::DependencyProperty^ Property;

		Value operator()(XamlFramework::DependencyObject^ dependencyObject) const
		{
			return static_cast<Value>(dependencyObject->GetValue(Property));
		}

		void operator()(XamlFramework::DependencyObject^ dependencyObject, Value newValue)
		{
			dependencyObject->SetValue(Property, newValue);
		}

		XamlFramework::DependencyProperty^ get_Property() const noexcept
		{
			return m_Property;
		}
	private:
		ValueChangedHandler m_ValueChangedHandler;

		XamlFramework::DependencyProperty^ m_Property;
	};
}
#else
#include <OneToolkit.h>
#include <winrt/OneToolkit.UI.Xaml.Controls.h>
#include <winrt/OneToolkit.UI.Xaml.Triggers.h>
#endif