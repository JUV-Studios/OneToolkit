#include "pch.h"
#include "ObservableBase.h"
#include "ObservableBase.g.cpp"

using namespace winrt;
using namespace Windows::UI::Xaml::Data;

namespace winrt::OneToolkit::Mvvm::implementation
{
	bool ObservableBase::Decide(hstring const&)
	{
		return true;
	}

	void ObservableBase::WhenPropertyChanged(PropertyChangedEventArgs const&)
	{
		// Do nothing here
	}

	void ObservableBase::Raise(hstring const& propertyName)
	{
		if (Decide(propertyName))
		{
			PropertyChangedEventArgs args { propertyName };
			m_PropertyChanged(*this, args);
			WhenPropertyChanged(args);
		}
	}

	event_token ObservableBase::PropertyChanged(PropertyChangedEventHandler const& handler)
	{
		return m_PropertyChanged.add(handler);
	}

	void ObservableBase::PropertyChanged(event_token token) noexcept
	{
		m_PropertyChanged.remove(token);
	}
}