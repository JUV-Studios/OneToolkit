// Ported from https://github.com/CommunityToolkit/WindowsCommunityToolkit/blob/rel/7.1.0/Microsoft.Toolkit.Uwp.UI/Triggers/FullScreenModeStateTrigger.cs.

#include "pch.h"
#ifdef framework_winui3
using namespace Microsoft::UI::Windowing;
#else
using namespace Windows::UI::ViewManagement;
#endif

using namespace AppFramework;
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
#ifdef framework_winui3
				// TODO implement FullScreenStateTrigger for WinUI 3.
				AppView->Changed += ref new TypedEventHandler<AppWindow^, AppWindowChangedEventArgs^>(this, &FullScreenStateTrigger::Changed);
#else
				AppView = ApplicationView::GetForCurrentView();
				AppView->VisibleBoundsChanged += ref new TypedEventHandler<ApplicationView^, Object^>(this, &FullScreenStateTrigger::OnVisibleBoundsChanged);
#endif
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
				if (AppView)
				{
#ifdef framework_winui3
					SetActive(value == (AppView->Presenter->Kind == AppWindowPresenterKind::FullScreen));
#else
					SetActive(value == AppView->IsFullScreenMode);
#endif
				}
			}
		}

#ifdef framework_winui3
		void Changed(AppWindow^ sender, AppWindowChangedEventArgs^ e)
		{
			SetActive(m_IsFullScreen == (AppView->Presenter->Kind == AppWindowPresenterKind::FullScreen));
		}
#else
		void OnVisibleBoundsChanged(ApplicationView^ sender, Object^ args)
		{
			SetActive(m_IsFullScreen == sender->IsFullScreenMode);
		}
#endif
	private:
		bool m_IsFullScreen;
#ifdef framework_winui3
		AppWindow^ AppView;
#else
		ApplicationView^ AppView;
#endif
	};
}