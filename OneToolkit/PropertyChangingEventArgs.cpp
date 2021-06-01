#include "pch.h"
#include "PropertyChangingEventArgs.h"
#include "Mvvm.PropertyChangingEventArgs.g.cpp"

namespace winrt::OneToolkit::Mvvm::implementation
{
	PropertyChangingEventArgs::PropertyChangingEventArgs(hstring const& propertyName) : m_PropertyName(propertyName)
	{
	}

	hstring PropertyChangingEventArgs::PropertyName() const noexcept
	{
		return m_PropertyName;
	}
}