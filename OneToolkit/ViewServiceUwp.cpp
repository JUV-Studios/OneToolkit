#include "pch.h"
#include "ViewServiceUwp.h"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Data;

namespace winrt::OneToolkit::UI::implementation
{
	hstring ViewServiceUwp::Title() const
	{
		return m_AppView.Title();
	}

	void ViewServiceUwp::Title(hstring const& value)
	{
		if (m_AppView.Title() != value)
		{
			m_AppView.Title(value);
			m_PropertyChanged(*this, PropertyChangedEventArgs(L"Title"));
		}
	}

	bool ViewServiceUwp::IsFullScreen() const
	{
		return m_AppView.IsFullScreenMode();
	}

	void ViewServiceUwp::IsFullScreen(bool value)
	{
		auto currentValue = m_AppView.IsFullScreenMode();
		if (currentValue != value)
		{
			if (value)
			{
				if (!m_AppView.TryEnterFullScreenMode()) return;
			}
			else m_AppView.ExitFullScreenMode();
			m_PropertyChanged(*this, PropertyChangedEventArgs(L"IsFullScreen"));
		}
	}

	IInspectable ViewServiceUwp::ReferenceSource() const noexcept
	{
		return m_CoreAppView;
	}

	IAsyncOperation<bool> ViewServiceUwp::CloseAsync()
	{
		return m_AppView.TryConsolidateAsync();
	}

	event_token ViewServiceUwp::PropertyChanged(PropertyChangedEventHandler const& handler)
	{
		return m_PropertyChanged.add(handler);
	}

	void ViewServiceUwp::PropertyChanged(event_token token) noexcept
	{
		m_PropertyChanged.remove(token);
	}
}