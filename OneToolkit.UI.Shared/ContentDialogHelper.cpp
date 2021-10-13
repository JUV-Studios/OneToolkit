#include "pch.h"

using namespace Platform;
using namespace Concurrency;
using namespace Windows::Foundation;
using namespace Framework;
using namespace Framework::Media;
using namespace Framework::Controls;

namespace Component::Controls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ContentDialogHelper sealed
	{
	public:
		static property bool IsShown
		{
			bool get()
			{
				return dynamic_cast<ContentDialog^>(VisualTreeHelper::GetOpenPopups(Window::Current)) != nullptr;
			}
		}

		static IAsyncOperation<ContentDialogResult>^ TryShowAsync(ContentDialog^ dialog, ContentDialogPlacement placement)
		{
			if (!IsShown) return dialog->ShowAsync(placement);
			else return create_async([]() { return ContentDialogResult::None; });
		}
	private:
		ContentDialogHelper();
	};
}