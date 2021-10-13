// Ported from https://github.com/CommunityToolkit/WindowsCommunityToolkit/blob/rel/7.1.0/Microsoft.Toolkit.Uwp.UI/Triggers/FullScreenModeStateTrigger.cs.

#include "pch.h"

using namespace Windows::UI::Xaml;
using namespace Windows::UI::ViewManagement;
using namespace Windows::Foundation;
using namespace Windows::ApplicationModel;

namespace OneToolkit::UI::Xaml::Triggers
{
	public ref class FullScreenStateTrigger sealed : StateTriggerBase
	{
	public:
		FullScreenStateTrigger()
		{
			if (!DesignMode::DesignModeEnabled)
			{
				AppView = ApplicationView::GetForCurrentView();
				AppView->VisibleBoundsChanged += ref new TypedEventHandler<ApplicationView^, Object^>(this, &FullScreenStateTrigger::OnVisibleBoundsChanged);
			}
		}

		property bool IsFullScreen
		{
			bool get()
			{
				return m_IsFullScreen;
			}

			void set(bool value)
			{
				m_IsFullScreen = value;
				if (AppView) SetActive(value == AppView->IsFullScreenMode);
			}
		}

		void OnVisibleBoundsChanged(ApplicationView^ sender, Object^ args)
		{
			SetActive(m_IsFullScreen == ApplicationView::GetForCurrentView()->IsFullScreenMode);
		}
	private:
		bool m_IsFullScreen;
		ApplicationView^ AppView;
	};
}