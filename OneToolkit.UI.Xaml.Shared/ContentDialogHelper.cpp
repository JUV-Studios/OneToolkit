#include "pch.h"

using namespace Platform;
using namespace Concurrency;
using namespace Windows::Foundation;
using namespace Framework;
using namespace Framework::Media;
using namespace Framework::Controls;

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
	private:
		ContentDialogHelper();
	};
}