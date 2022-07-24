#pragma once
#define DeclareDependencyProperty(Type, Name) private: static ::winrt::XamlFramework::DependencyProperty s_##Name##Property;\
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