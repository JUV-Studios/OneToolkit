#include "pch.h"
#include "ObservableBase.h"
#include "ObservableBase.g.cpp"
#include "ObservableBaseHelper.g.cpp"

using namespace winrt;
using namespace Windows::UI::Xaml::Data;

namespace winrt::OneToolkit::Mvvm::implementation
{
	bool ObservableBase::SuppressEvents() const noexcept
	{
		return m_SuppressEvents;
	}

	void ObservableBase::SuppressEvents(bool value)
	{
		if (m_SuppressEvents != value)
		{
			m_SuppressEvents = value;
			m_PropertyChanged(*this, PropertyChangedEventArgs(L"m_SuppressEvents"));
		}
	}

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

	void ObservableBaseHelper::Raise(OneToolkit::Mvvm::ObservableBase const& instance, hstring const& propertyName)
	{
		get_self<ObservableBase>(instance)->Raise(propertyName);
	}
}