#include "pch.h"

using namespace concurrency;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Metadata;
using namespace Windows::UI::Xaml::Controls;

namespace OneToolkit::UI::Xaml::Controls
{
	[WebHostHidden]
	public delegate IAsyncAction^ ContentDialogDisplayCompletionAsyncHandler(ContentDialog^ dialog, ContentDialogResult result);

	[WebHostHidden]
	public ref class ContentDialogHelper sealed
	{
	public:
		IAsyncOperation<bool>^ TryShowAsync(ContentDialog^ dialog, ContentDialogPlacement placement, ContentDialogDisplayCompletionAsyncHandler^ completionHandler)
		{
			return TryShowAsync(dialog, placement, completionHandler, ViewService::GetForCurrentView());
		}

		IAsyncOperation<bool>^ TryShowAsync(ContentDialog^ dialog, ContentDialogPlacement placement, ContentDialogDisplayCompletionAsyncHandler^ completionHandler, IViewServiceProvider^ viewServiceProvider)
		{
			return create_async([=]() -> task<bool>
				{
					if (!viewServiceProvider->IsDialogShown)
					{
						viewServiceProvider->IsDialogShown = true;
						auto result = co_await dialog->ShowAsync(placement);
						co_await completionHandler(dialog, result);
						viewServiceProvider->IsDialogShown = false;
						co_return true;
					}

					co_return false;
				});
		}
	private:
		ContentDialogHelper();
	};
}