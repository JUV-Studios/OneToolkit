#include "pch.h"

using namespace concurrency;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Controls;

namespace OneToolkit::UI::Xaml::Controls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public delegate IAsyncAction^ ContentDialogDisplayCompletionAsyncHandler(ContentDialog^ dialog, ContentDialogResult result);

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

		static IAsyncOperation<bool>^ TryShowAsync(ContentDialog^ dialog, ContentDialogPlacement placement, ContentDialogDisplayCompletionAsyncHandler^ completionHandler)
		{
			return create_async([=]() -> task<bool>
				{
					if (!IsShown)
					{
						auto result = co_await dialog->ShowAsync(placement);
						co_await completionHandler(dialog, result);
						co_return true;
					}

					co_return false;
				});
		}
	private:
		ContentDialogHelper();
	};
}